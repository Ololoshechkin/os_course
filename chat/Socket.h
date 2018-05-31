//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKET_H
#define HW5_CLIENT_SERVER_SOCKET_H

#include <cstddef>
#include <memory>

#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include "SocketScopedBase.h"

class ServerSocket;

class Socket : public SocketScopedBase, public std::enable_shared_from_this<Socket> {
 private:
  explicit Socket(int socket_fd);
  friend ServerSocket;
 public:
  explicit Socket();
  explicit Socket(InetSocketAddress const& address);
  std::string ReadPacket(std::size_t length) const;
  void WriteBytes(const std::string& bytes) const;
  ~Socket();
};

#endif //HW5_CLIENT_SERVER_SOCKET_H
