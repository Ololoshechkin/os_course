//
// Created by Vadim on 28/05/2018.
//

#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include "KVDatabaseServer.h"
#include "ZygoteDatabaseService.h"
#include "ExceptionHelp.h"
#include "KVClient.h"

const std::string KVDatabaseServer::kZygoteAddressSufix = "ZygoteDatabase.sock"; // NOLINT

KVDatabaseServer::KVDatabaseServer(const std::string& address) :
        zygote_address(address + "." + kZygoteAddressSufix) {
  server_socket.BindAndListen(address + ".sock");
  ScopedUnixSocket zygote_side_client_socket;
  zygote_side_client_socket.Connect(address + ".sock");
  zygote_socket = std::move(server_socket.Accept());
  const auto pid = fork();
  if (pid == 0) { // child(zygote) code
    ZygoteDatabaseService zygote_service(
            std::move(zygote_side_client_socket));
    zygote_service.Run();
  } else if (pid < 0) { // error
    throw std::runtime_error(
            GetErrorMessage("failed to fork zygote process"));
  }
}

void KVDatabaseServer::Run() {
  event_manager.SubscribeToSignal(
          SIGINT, [this] {
            std::cout << std::endl << "exit" << std::endl;
            this->~KVDatabaseServer();
            exit(0);
          });
  server_socket.SubscribeToRead(
          event_manager, [this] {
            auto client_socket = server_socket.Accept();
            ProcessClient(std::move(client_socket));
          });
  event_manager.Start();
}

void KVDatabaseServer::ProcessClient(
        ScopedUnixSocket&& client_socket
) {
  zygote_socket.SendMessage(kFork);
  auto send_pipe = zygote_socket.ReceivePipe();
  auto receive_pipe = zygote_socket.ReceivePipe();
  client_socket.SendPipe(send_pipe);
  client_socket.SendPipe(receive_pipe);
}

KVDatabaseServer::~KVDatabaseServer() {
  zygote_socket.SendMessage(kTerminate);
}
