//
// Created by Vadim on 14/04/2018.
//

#include <cstddef>
#include <iostream>
#include "Socket.h"
#include "ExceptionHelp.h"

const std::size_t kBufSize = 1024;

Socket::Socket(int socket_fd) :
        SocketScopedBase(socket_fd) {
}

Socket::Socket(InetSocketAddress const& address) {
  auto system_address = address.ToSystemSocketAddress();
  if (connect(socket_fd, (struct sockaddr*) &system_address,
              sizeof(system_address)) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to connect"));
  }
}

std::string Socket::ReadPacket(std::size_t length) const {
  char buf[kBufSize];
  std::string bytes;
  while (length != 0) {
    std::size_t bytes_to_read = std::min(length, kBufSize);
    if (recv(socket_fd, buf, bytes_to_read, 0) < 0) {
      throw std::runtime_error(GetErrorMessage("failed to receive bytes"));
    }
    length -= bytes_to_read;
    for (std::size_t i = 0; i < bytes_to_read; ++i) {
      bytes.push_back(buf[i]);
    }
  }
  return bytes;
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
  return Event(socket_fd,
               {Event::EventType::kInput, Event::EventType::kDisconnect,
                Event::EventType::kError});
}

Event Socket::GetSendEvent() {
  return Event(socket_fd,
               {Event::EventType::kOutput, Event::EventType::kDisconnect,
                Event::EventType::kError});
}

Event Socket::GetSendAndReceiveEvent() {
  return Event(socket_fd, {Event::EventType::kInput, Event::EventType::kOutput,
                           Event::EventType::kDisconnect,
                           Event::EventType::kError});
}

std::string Socket::ReadBytes() const {
  static char buf[kBufSize];
  std::string received_bucket;
  while (true) {
    const auto received_count = recv(socket_fd, buf, kBufSize, 0);
    if (received_count <= 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        break;
      } else {
        throw std::runtime_error(GetErrorMessage("failed to receive bytes"));
      }
    }
    for (size_t i = 0; i < received_count; ++i) {
      received_bucket.push_back(buf[i]);
    }
  }
  return received_bucket;
}

Socket::~Socket() = default;
