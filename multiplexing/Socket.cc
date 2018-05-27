//
// Created by Vadim on 14/04/2018.
//

#include <cstddef>
#include <iostream>
#include <sys/select.h>
#include "Socket.h"
#include "ExceptionHelp.h"

const std::size_t kBufSize = 1024;

Socket::Socket(int socket_fd) :
        SocketScopedBase(socket_fd) {
}

Socket::Socket(InetSocketAddress const& address) :
        address(address) {
}

void Socket::TryWriteBytes(std::string& bytes) const {
  while (!bytes.empty()) {
    const auto send_result = send(socket_fd, bytes.c_str(), bytes.size(), 0);
    if (send_result < 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        errno = 0;
        break;
      }
      throw std::runtime_error(GetErrorMessage("failed to send bytes"));
    }
    auto position = static_cast<std::size_t>(send_result);
    bytes = bytes.substr(position, bytes.size() - position);
  }
}

Socket::Socket() :
        SocketScopedBase(-1) {
}

Event Socket::GetReceiveEvent() {
  return Event(
          socket_fd, {Event::EventType::kInput, Event::EventType::kDisconnect,
                      Event::EventType::kError});
}

Event Socket::GetSendEvent() {
  return Event(
          socket_fd, {Event::EventType::kOutput, Event::EventType::kDisconnect,
                      Event::EventType::kError});
}

Event Socket::GetSendAndReceiveEvent() {
  return Event(
          socket_fd, {Event::EventType::kInput, Event::EventType::kOutput,
                      Event::EventType::kDisconnect, Event::EventType::kError});
}

std::string Socket::ReadBytes() const {
  std::cout << "reading bytes..." << std::endl;
  char buf[kBufSize];
  std::string received_bucket;
  while (true) {
    std::cout << " recv(socket_fd, buf, kBufSize, 0);" << std::endl;
    const auto received_count = recv(socket_fd, buf, kBufSize, 0);
    std::cout << " received_count = " << received_count << std::endl;
    if (received_count <= 0) {
      if (received_count == 0 || (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        std::cout << "(errno == EAGAIN) || (errno == EWOULDBLOCK) => break"
                  << std::endl;
        errno = 0;
        break;
      } else {
        std::cout << "exception =(" << std::endl;
        throw std::runtime_error(GetErrorMessage("failed to receive bytes"));
      }
    }
    for (size_t i = 0; i < received_count; ++i) {
      std::cout << "received_bucket.push_back( " << buf[i] << " );"
                << std::endl;
      received_bucket.push_back(buf[i]);
    }
  }
  std::cout << "return received_bucket;" << std::endl;
  return received_bucket;
}

bool Socket::Connect() {
  auto system_address = address.ToSystemSocketAddress();
  const auto result = connect(
          socket_fd, (struct sockaddr*) &system_address,
          sizeof(system_address));
  if (result == 0 || errno == EINPROGRESS) {
    return false;
  }
  if (result < 0) {
    throw std::runtime_error(GetErrorMessage("failed to connect"));
  }
  return true;
}

Socket::~Socket() = default;
