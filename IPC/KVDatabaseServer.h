//
// Created by Vadim on 28/05/2018.
//

#ifndef IPC_MAINSERVER_H
#define IPC_MAINSERVER_H

#include <string>
#include "ScopedUnixSocket.h"

class KVDatabaseServer {
 private:
  ScopedUnixSocket server_socket;
  std::string zygote_address;
 public:
  static const std::string kZygoteAddressSufix;
  explicit KVDatabaseServer(const std::string& address); // init database
  ~KVDatabaseServer();
  void Run();
  void ProcessClient(const ScopedUnixSocket& client_socket);
};

#endif //IPC_MAINSERVER_H
