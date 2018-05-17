//
// Created by Vadim on 14/04/2018.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
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

ServerSocket::ServerSocket(InetSocketAddress const& address) :
        SocketBase() {
  // Зачем явно звать конструктор базового класса?
  auto system_address = address.ToSystemSocketAddress();
  // почему не const auto?

  if (bind(socket_fd, (const sockaddr*) &system_address,
           sizeof(system_address)) < 0) {
    perror("failed to bind to given address");
    throw std::exception();
    // Какое сообщение получит обработчик исключения?
  }
  listen(socket_fd, InetUtils::kDefaultBacklogSize);
  // Не проверяется сообщение об ошибке
}

std::shared_ptr<Socket> ServerSocket::accept() {
  int client_socket_fd = ::accept(socket_fd, nullptr, nullptr);
  if (client_socket_fd < 0) {
    perror("failed to accept a new client");
    throw std::exception();
    // Общее соображение про ошибку исключения
  }
  std::cout << "client_socket_fd = " << client_socket_fd << '\n';
  // why not std::endl?
  return std::shared_ptr<Socket>(new Socket(client_socket_fd));
  // 1. Resource leakage - what if new throws - you will leak client_socket_fd
  // 2. Performance: why not std::make_shared?
}
