//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKET_H
#define HW5_CLIENT_SERVER_SOCKET_H

#include "SocketBase.h"
#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include <memory>
#include <cstddef>

class ServerSocket;

class Socket : public SocketBase, public std::enable_shared_from_this<Socket> {
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
