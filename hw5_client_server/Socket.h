//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKET_H
#define HW5_CLIENT_SERVER_SOCKET_H

#include "SocketBase.h"
#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

class ServerSocket;

class ByteReadChanel;

class ByteWriteChanel;

class Socket : public SocketBase, public std::enable_shared_from_this<Socket> {
 private:
  explicit Socket(int socket_fd);
  friend ServerSocket;
  friend ByteReadChanel;
  friend ByteWriteChanel;
 public:
  explicit Socket();
  explicit Socket(InetSocketAddress const& address);
  std::string ReadPacket(size_t length);
  void WriteBytes(const std::string& bytes);
  int GetFd() const;
  ~Socket();
};

#endif //HW5_CLIENT_SERVER_SOCKET_H
