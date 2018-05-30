//
// Created by Vadim on 28/05/2018.
//

#include <zconf.h>
#include <libnet.h>
#include <iostream>
#include "KVDatabaseServer.h"
#include "ZygoteDatabaseService.h"
#include "ExceptionHelp.h"

const std::string KVDatabaseServer::kZygoteAddressSufix = "ZygoteDatabase.sock"; // NOLINT

KVDatabaseServer::KVDatabaseServer(const std::string& address) :
        zygote_address(address + "." + kZygoteAddressSufix) {
  std::cout << "binding(kv) to " << address << std::endl;
  server_socket.BindAndListen(address);
  std::cout << "bind ok(kv)" << std::endl;
  auto zygote_server_socket = ScopedUnixSocket();
  std::cout << "binding(zygote)" << std::endl;
  zygote_server_socket.BindAndListen(zygote_address);
  std::cout << "bind ok(zygote)" << std::endl;
  const auto pid = fork();
  if (pid == 0) { // child(zygote) code
    auto zygote_service = ZygoteDatabaseService(zygote_server_socket);
    zygote_service.Run();
  } else if (pid < 0) { // error
    throw std::runtime_error(
            GetErrorMessage("failed to fork zygote process"));
  }
}

void KVDatabaseServer::Run() {
  while (true) {
    const auto client_socket = server_socket.Accept();
    ProcessClient(client_socket);
  }
}

void KVDatabaseServer::ProcessClient(const ScopedUnixSocket& client_socket) {
  auto zygote_client_socket = ScopedUnixSocket();
  std::cout << "connecting zygote_client_socket to : " << zygote_address
            << std::endl;
  zygote_client_socket.Connect(zygote_address);
  std::cout << "connected" << std::endl;
  client_socket.SendFDImage(zygote_client_socket.ToImage());
}

// NOLINT
KVDatabaseServer::~KVDatabaseServer() = default;
