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

bool Socket::TryWriteBytes(const std::string& bytes) const {
  if (send(socket_fd, bytes.c_str(), bytes.size(), 0) < 0) {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
      return false;
    }
    throw std::runtime_error(GetErrorMessage("failed to send bytes"));
  }
  return true;
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
  static char buf[kBufSize];
  std::string received_bucket;
  while (true) {
    std::cout << " recv(socket_fd, buf, kBufSize, 0);" << std::endl;
    const auto received_count = recv(socket_fd, buf, kBufSize, 0);
    std::cout << " received_count = " << received_count << std::endl;
    if (received_count <= 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        std::cout << "(errno == EAGAIN) || (errno == EWOULDBLOCK) => break"
                  << std::endl;
        break;
      } else {
        std::cout << "exception =(" << std::endl;
        throw std::runtime_error(GetErrorMessage("failed to receive bytes"));
      }
    }
    for (size_t i = 0; i < received_count; ++i) {
      std::cout << "received_bucket.push_back(buf[" << i << "]);" << std::endl;
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
