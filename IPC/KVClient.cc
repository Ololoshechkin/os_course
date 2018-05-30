//
// Created by Vadim on 29/05/2018.
//

#include <vector>
#include <iostream>
#include "KVClient.h"

const std::string KVClient::kSetValue = "set"; // NOLINT
const std::string KVClient::kGetValue = "get"; // NOLINT
const std::string KVClient::kGetKeys = "keyset"; // NOLINT
const std::string KVClient::kDisconnect = "disconnect"; // NOLINT

KVClient::KVClient(const std::string& server_address) {
  ScopedUnixSocket socket_to_server;
  std::cout << "server_address = " << server_address << std::endl;
  socket_to_server.Connect(server_address);
  std::cout << "connected" << std::endl;
  const auto zygote_socket = socket_to_server.ReceiveFDImage().ToUnixSocket();
  std::cout << "zygote_socket OK" << std::endl;
  service_socket = zygote_socket.ReceiveFDImage().ToUnixSocket();
  std::cout << "service_socket OK" << std::endl;
}

void KVClient::Set(const std::string& key, const std::string& value) {
  service_socket.SendMessage(kSetValue);
  service_socket.SendMessage(key);
  service_socket.SendMessage(value);
}

std::string KVClient::Get(const std::string& key) {
  service_socket.SendMessage(kSetValue);
  service_socket.SendMessage(key);
  return service_socket.ReceiveMessage();
}

std::vector<std::string> KVClient::GetKeys() {
  service_socket.SendMessage(kGetKeys);
  std::vector<std::string> keys;
  size_t length = service_socket.ReceiveSizeT();
  for (size_t i = 0; i < length; ++i) {
    keys.push_back(service_socket.ReceiveMessage());
  }
  return keys;
}

KVClient::~KVClient() {
  service_socket.SendMessage(kDisconnect);
}
