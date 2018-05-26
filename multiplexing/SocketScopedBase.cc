//
// Created by Vadim on 15/04/2018.
//

#include <sys/socket.h>
#include <cstdio>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <sys/fcntl.h>
#include "SocketScopedBase.h"
#include "ExceptionHelp.h"

SocketScopedBase::SocketScopedBase() :
        SocketScopedBase(socket(AF_INET, SOCK_STREAM, 0)) {
  if (socket_fd < 0) {
    throw std::runtime_error(GetErrorMessage("failed to create socket"));
  }
}

SocketScopedBase::SocketScopedBase(int socket_fd) :
        socket_fd(socket_fd) {
}

SocketScopedBase::~SocketScopedBase() {
  close(socket_fd);
}

void SocketScopedBase::SetBlocking(bool blocking) {
  auto flags = fcntl(socket_fd, F_GETFL);
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  if (fcntl(socket_fd, F_SETFL, flags) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to change blocking type"));
  }
}
