//
// Created by Vadim on 14/04/2018.
//

#include <cstddef>
#include <iostream>

#include "Socket.h"

const std::size_t kBufSize = 1024;

Socket::Socket(int socket_fd) :
        SocketScopedBase(socket_fd) {
//    std::cerr << "Socket() on fd=" << socket_fd << '\n';
}

Socket::Socket(InetSocketAddress const& address) :
        SocketScopedBase() {
//    std::cerr << "Socket() on fd=" << socket_fd << '\n';
  auto system_address = address.ToSystemSocketAddress();
  if (connect(socket_fd, (struct sockaddr*) &system_address,
              sizeof(system_address)) < 0) {
    perror("connection failed");
    throw std::exception();
  }
}

std::string Socket::ReadPacket(std::size_t length) const {
  char buf[kBufSize];
  std::string bytes;
  while (length != 0) {
    std::size_t bytes_to_read = std::min(length, kBufSize);
    if (recv(socket_fd, buf, bytes_to_read, 0) < 0) {
      perror("failed to read bytes");
      throw std::exception();
    }
    length -= bytes_to_read;
    for (std::size_t i = 0; i < bytes_to_read; ++i) {
      bytes.push_back(buf[i]);
    }
  }
  return bytes;
}

void Socket::WriteBytes(const std::string& bytes) const {
  send(socket_fd, bytes.c_str(), bytes.size(), 0);
}

Socket::Socket() :
        SocketScopedBase(-1) {
//    std::cerr << "~Socket() on fd=" << socket_fd << '\n';
}


Socket::~Socket() = default;
