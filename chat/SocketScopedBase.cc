//
// Created by Vadim on 15/04/2018.
//

#include <sys/socket.h>
#include <cstdio>
#include <exception>
#include <iostream>
#include <unistd.h>
#include "SocketScopedBase.h"

SocketScopedBase::SocketScopedBase() :
        SocketScopedBase(socket(AF_INET, SOCK_STREAM, 0)) {
  if (socket_fd < 0) {
    perror("failed to open server_socket_");
    throw std::exception();
  }
}

SocketScopedBase::SocketScopedBase(int socket_fd) :
        socket_fd(socket_fd) {
}

SocketScopedBase::~SocketScopedBase() {
  close(socket_fd);
}