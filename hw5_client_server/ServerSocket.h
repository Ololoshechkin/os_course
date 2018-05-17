//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SERVERSOCKET_H
#define HW5_CLIENT_SERVER_SERVERSOCKET_H

#include "InetSocketAddress.h"
#include "Socket.h"
#include "SocketBase.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// Заголовочные файлы

class Socket;

// https://google.github.io/styleguide/cppguide.html#Forward_Declarations

class ServerSocket : public SocketBase {
 public:
  explicit ServerSocket(InetSocketAddress const& address);
  std::shared_ptr<Socket> accept();
};

#endif //HW5_CLIENT_SERVER_SERVERSOCKET_H
