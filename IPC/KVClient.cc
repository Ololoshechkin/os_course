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
  socket_to_server.Connect(server_address + ".sock");
  receive_pipe = std::move(socket_to_server.ReceivePipe());
  send_pipe = std::move(socket_to_server.ReceivePipe());
}

void KVClient::Set(const std::string& key, const std::string& value) {
  send_pipe.SendMessage(kSetValue);
  send_pipe.SendMessage(key);
  send_pipe.SendMessage(value);
}

std::string KVClient::Get(const std::string& key) {
  send_pipe.SendMessage(kGetValue);
  send_pipe.SendMessage(key);
  return receive_pipe.ReceiveMessage();
}

std::vector<std::string> KVClient::GetKeys() {
  send_pipe.SendMessage(kGetKeys);
  std::vector<std::string> keys;
  size_t length = receive_pipe.ReceiveSizeT();
  for (size_t i = 0; i < length; ++i) {
    keys.push_back(receive_pipe.ReceiveMessage());
  }
  return keys;
}

KVClient::~KVClient() {
  send_pipe.SendMessage(kDisconnect);
}
