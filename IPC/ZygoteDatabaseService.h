//
// Created by Vadim on 28/05/2018.
//

#ifndef IPC_ZYGOTEDATABASESERVICE_H
#define IPC_ZYGOTEDATABASESERVICE_H
static const char* const kTerminate = "terminate";
static const char* const kFork = "fork";

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
  ScopedUnixSocket main_server_client_socket;
  std::string GenerateForkArrdess() const;
 public:
  explicit ZygoteDatabaseService(
          ScopedUnixSocket&& main_server_client_socket
  );
  ~ZygoteDatabaseService();
  void Run();
  void ExecuteIndependentFork(
          ScopedPipe&& send_pipe, ScopedPipe&& receive_pipe
  );
};

#endif //IPC_ZYGOTEDATABASESERVICE_H
