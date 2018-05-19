//
// Created by Vadim on 15/04/2018.
//

#include <sys/socket.h>
#include <cstdio>
#include <exception>
#include <iostream>
#include <unistd.h>
#include "SocketBase.h"

SocketBase::SocketBase() :
        SocketBase(socket(AF_INET, SOCK_STREAM, 0)) {
  if (socket_fd < 0) {
    perror("failed to open server_socket_");
    throw std::exception();
  }
}

SocketBase::SocketBase(int socket_fd) :
        socket_fd(socket_fd) {
}

SocketBase::~SocketBase() {
  close(socket_fd);
}