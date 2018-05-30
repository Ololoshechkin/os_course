//
// Created by Vadim on 28/05/2018.
//

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "ZygoteDatabaseService.h"
#include "KVDatabaseServer.h"
#include "ExceptionHelp.h"
#include "KVClient.h"

ZygoteDatabaseService::ZygoteDatabaseService(
        ScopedUnixSocket&& main_server_client_socket
) :
        database_keys(
                {"a", "b", "hello", "goodbye", "kotlin", "native"}),
        main_server_client_socket(std::move(main_server_client_socket)) {
  database["a"] = "abacaba";
  database["b"] = "abacabadabacabaeabacabadabadcaba";
  database["hello"] = "hi";
  database["goodbye"] = "bye";
  database["kotlin"] = "native!";
  database["native"] = "kotlin!";
}

ZygoteDatabaseService::~ZygoteDatabaseService() = default;

void ZygoteDatabaseService::Run() {
  while (true) {
    const auto comand = main_server_client_socket.ReceiveMessage();
    if (comand == kTerminate) {
      break;
    } else if (comand == kFork) {
      ScopedPipe send_pipe, receive_pipe;
      main_server_client_socket.SendPipe(send_pipe);
      main_server_client_socket.SendPipe(receive_pipe);
      int pid = fork();
      if (pid == 0) {  // fork's code
        ExecuteIndependentFork(std::move(send_pipe), std::move(receive_pipe));
      } else if (pid < 0) {  // error while fork
        throw std::runtime_error(
                GetErrorMessage(
                        "failed to fork a process for new service instance"));
      }
    }
  }
}

void ZygoteDatabaseService::ExecuteIndependentFork(
        ScopedPipe&& send_pipe, ScopedPipe&& receive_pipe
) {
  while (true) {
    const auto comand = receive_pipe.ReceiveMessage();
    if (comand == KVClient::kDisconnect) {
      break;
    } else if (comand == KVClient::kSetValue) {
      const auto key = receive_pipe.ReceiveMessage();
      const auto value = receive_pipe.ReceiveMessage();
      if (!database.count(key)) {
        database_keys.push_back(key);
      }
      database[key] = value;
    } else if (comand == KVClient::kGetValue) {
      const auto key = receive_pipe.ReceiveMessage();
      send_pipe.SendMessage(database[key]);
    } else if (comand == KVClient::kGetKeys) {
      send_pipe.SendSizeT(database_keys.size());
      std::for_each(
              database_keys.begin(), database_keys.end(),
              [&send_pipe](const std::string& key) {
                send_pipe.SendMessage(key);
              });
    } else {
      std::cout << "invalid comand : " << comand << std::endl;
      break;
    }
  }
}

std::string ZygoteDatabaseService::GenerateForkArrdess() const {
  auto address = KVDatabaseServer::kZygoteAddressSufix + ".fork";
  for (int i = 0; i < 25; ++i) {
    address.push_back((char) (rand() % std::numeric_limits<char>::max()));
  }
  address += ".sock";
  return address;
}
