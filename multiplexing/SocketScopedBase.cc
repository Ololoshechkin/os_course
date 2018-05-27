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
  auto flags = fcntl(socket_fd, F_GETFL, 0);
  if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to change blocking type"));
  }
}

SocketScopedBase::~SocketScopedBase() {
  close(socket_fd);
}
