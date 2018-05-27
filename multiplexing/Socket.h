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
#include "ScopedMultiplexer.h"

class ServerSocket;

class Socket : public SocketScopedBase, public std::enable_shared_from_this<
        Socket
> {
 private:
  explicit Socket(int socket_fd);
  friend ServerSocket;
  InetSocketAddress address;
 public:
  Socket();
  explicit Socket(InetSocketAddress const& address);
  bool Connect();
  std::string ReadBytes() const;
  void TryWriteBytes(
          std::string& bytes
  ) const;  // returns string sufix that is not sent
  Event GetReceiveEvent();
  Event GetSendEvent();
  Event GetSendAndReceiveEvent();
  ~Socket() override;
};

#endif //HW5_CLIENT_SERVER_SOCKET_H
