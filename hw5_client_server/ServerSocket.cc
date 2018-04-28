//
// Created by Vadim on 14/04/2018.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "ServerSocket.h"

ServerSocket::ServerSocket(InetSocketAddress const& address) :
        SocketBase() {
  auto system_address = address.ToSystemSocketAddress();
  if (bind(socket_fd, (const sockaddr*) &system_address,
           sizeof(system_address)) < 0) {
    perror("failed to bind to given address");
    throw std::exception();
  }
  listen(socket_fd, InetUtils::kDefaultBacklogSize);
}

std::shared_ptr<Socket> ServerSocket::accept() {
  int client_socket_fd = ::accept(socket_fd, nullptr, nullptr);
  if (client_socket_fd < 0) {
    perror("failed to accept a new client");
    throw std::exception();
  }
  std::cout << "client_socket_fd = " << client_socket_fd << '\n';
  return std::shared_ptr<Socket>(new Socket(client_socket_fd));
}
