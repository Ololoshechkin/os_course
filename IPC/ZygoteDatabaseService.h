//
// Created by Vadim on 28/05/2018.
//

#ifndef IPC_ZYGOTEDATABASESERVICE_H
#define IPC_ZYGOTEDATABASESERVICE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "ScopedUnixSocket.h"

class ZygoteDatabaseService {
 private:
  std::unordered_map<
          std::string, std::string
  > database;  // global initial database 
  std::vector<std::string> database_keys;
  ScopedUnixSocket zygote_server_socket;
  std::string GenerateForkArrdess() const;
 public:
  explicit ZygoteDatabaseService(const ScopedUnixSocket& zygote_server_socket);
  ~ZygoteDatabaseService();
  void Run();
  void ExecuteIndependentFork(const ScopedUnixSocket& fork_server_socket);
};

#endif //IPC_ZYGOTEDATABASESERVICE_H
