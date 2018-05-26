//
// Created by Vadim on 15/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SocketScopedBase_H
#define HW5_CLIENT_SERVER_SocketScopedBase_H

#include <memory.h>
#include <memory>

class SocketScopedBase {
 protected:
  int socket_fd;
  SocketScopedBase();
  explicit SocketScopedBase(int socket_fd);
  virtual ~SocketScopedBase();
 public:
  void SetBlocking(bool);
  
  virtual bool operator==(const SocketScopedBase& other) const {
    return socket_fd == other.socket_fd;
  }
};

#endif //HW5_CLIENT_SERVER_SocketScopedBase_H
