//
// Created by Vadim on 29/05/2018.
//

#ifndef IPC_KVCLIENT_H
#define IPC_KVCLIENT_H

#include <vector>
#include "ScopedUnixSocket.h"

class KVClient {
 private:
  ScopedUnixSocket service_socket;
 public:
  static const std::string kSetValue;
  static const std::string kGetValue;
  static const std::string kGetKeys;
  static const std::string kDisconnect;
  explicit KVClient(const std::string& server_address);
  ~KVClient();
  void Set(const std::string& key, const std::string& value);
  std::string Get(const std::string& key);
  std::vector<std::string> GetKeys();
};

#endif //IPC_KVCLIENT_H
