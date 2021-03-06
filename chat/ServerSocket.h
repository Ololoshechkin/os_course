//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SERVERSOCKET_H
#define HW5_CLIENT_SERVER_SERVERSOCKET_H

#include <memory>

#include "InetSocketAddress.h"
#include "Socket.h"
#include "SocketScopedBase.h"

class Socket;

class ServerSocket : public SocketScopedBase {
 public:
  explicit ServerSocket(InetSocketAddress const& address);
  std::shared_ptr<Socket> accept();
};

#endif //HW5_CLIENT_SERVER_SERVERSOCKET_H
