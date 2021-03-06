//
// Created by Vadim on 14/04/2018.
//

#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>

#include "ServerSocket.h"

ServerSocket::ServerSocket(InetSocketAddress const& address) {
  const auto system_address = address.ToSystemSocketAddress();
  if (bind(socket_fd, (const sockaddr*) &system_address,
           sizeof(system_address)) < 0) {
    perror("failed to bind to given address");
    throw std::exception();  // TODO
  }
  if (listen(socket_fd, InetUtils::kDefaultBacklogSize) < 0) {
    perror("failed to listen on given address");
    throw std::exception();  // TODO
  }
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
