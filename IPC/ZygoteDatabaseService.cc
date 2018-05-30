//
// Created by Vadim on 28/05/2018.
//

#include <cstdlib>
#include <zconf.h>
#include "ZygoteDatabaseService.h"
#include "KVDatabaseServer.h"
#include "ExceptionHelp.h"
#include "KVClient.h"

ZygoteDatabaseService::ZygoteDatabaseService(
        const ScopedUnixSocket& zygote_server_socket
) :
        database_keys(
                {"a", "b", "hello", "goodbye", "kotlin", "native", "Kohass"}),
        zygote_server_socket(zygote_server_socket) {
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
    const auto client_socket = zygote_server_socket.Accept();
    ScopedUnixSocket fork_server_socket;
    const auto fork_address = GenerateForkArrdess();
    fork_server_socket.BindAndListen(fork_address);
    ScopedUnixSocket fork_client_socket;
    fork_client_socket.Connect(fork_address); // TODO: pipes!
    int pid = fork();
    if (pid == 0) { // fork's code
      ExecuteIndependentFork(fork_server_socket);
    } else if (pid > 0) { // our code
      client_socket.SendFDImage(fork_client_socket.ToImage());
    } else { // error while fork
      throw std::runtime_error(
              GetErrorMessage(
                      "failed to fork a process for new service instance"));
    }
  }
}

void ZygoteDatabaseService::ExecuteIndependentFork(
        const ScopedUnixSocket& fork_server_socket
) {
  const auto socket = fork_server_socket.Accept();
  while (true) {
    const auto comand = socket.ReceiveMessage();
    if (comand == KVClient::kDisconnect) {
      break;
    } else if (comand == KVClient::kSetValue) {
      const auto key = socket.ReceiveMessage();
      const auto value = socket.ReceiveMessage();
      if (!database.count(key)) {
        database_keys.push_back(key);
      }
      database[key] = value;
    } else if (comand == KVClient::kGetValue) {
      const auto key = socket.ReceiveMessage();
      socket.SendMessage(database[key]);
    } else if (comand == KVClient::kGetKeys) {
      socket.SendSizeT(database_keys.size());
      std::for_each(
              database_keys.begin(), database_keys.end(),
              [&socket](const std::string& key) {
                socket.SendMessage(key);
              });
    } else {
      throw std::runtime_error("invalid comand!!!");
    }
  }
}

std::string ZygoteDatabaseService::GenerateForkArrdess() const {
  auto address = KVDatabaseServer::kZygoteAddressSufix + ".fork";
  for (int i = 0; i < 10; ++i) {
    address.push_back((char) (arc4random() % std::numeric_limits<char>::max()));
  }
  address += ".sock";
  return address;
}
