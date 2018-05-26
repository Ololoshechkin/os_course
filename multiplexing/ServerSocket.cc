//
// Created by Vadim on 14/04/2018.
//

#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include "ServerSocket.h"
#include "ExceptionHelp.h"

const int ServerSocket::kDefaultBacklogSize = 28;

ServerSocket::ServerSocket(InetSocketAddress const& address) {
  const auto system_address = address.ToSystemSocketAddress();
  if (bind(
          socket_fd, (const sockaddr*) &system_address,
          sizeof(system_address)) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to bind"));
  }
  if (listen(socket_fd, kDefaultBacklogSize) < 0) {
    throw std::runtime_error(GetErrorMessage("listning failed"));
  }
}

std::shared_ptr<Socket> ServerSocket::accept() {
  int client_socket_fd = ::accept(socket_fd, nullptr, nullptr);
  if (client_socket_fd < 0) {
    throw std::runtime_error(GetErrorMessage("failed to accept a new socket"));
  }
  std::cout << "accepted client_socket_fd = " << client_socket_fd << std::endl;
  return std::shared_ptr<Socket>(new Socket(client_socket_fd));
}

Event ServerSocket::GetAcceptEvent() {
  return Event(socket_fd, {Event::EventType::kInput});
}
