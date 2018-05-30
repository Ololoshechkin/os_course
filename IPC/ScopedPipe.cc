//
// Created by Vadim on 30/05/2018.
//

#include <algorithm>
#include <iostream>
#include <libnet.h>
#include <zconf.h>
#include "ScopedPipe.h"
#include "ExceptionHelp.h"

ScopedPipe::ScopedPipe() {
  if (pipe(fd) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to create pipe"));
  }
}

ScopedPipe::~ScopedPipe() = default;

int ScopedPipe::FdFrom() const {
  return fd[1];
}

int ScopedPipe::FdTo() const {
  return fd[0];
}

inline std::size_t StringToSizeT(const std::string& s) {
  std::size_t int_val = 0;
  for (std::size_t i = 0; i < 4; ++i) {
    const auto cur = static_cast<std::size_t>(s[i]);
    int_val = (int_val << 8) | cur;
  }
  return int_val;
}

inline std::string SizeTToString(std::size_t length) {
  return {static_cast<char>((length >> 24) & 0xFF),
          static_cast<char>((length >> 16) & 0xFF),
          static_cast<char>((length >> 8) & 0xFF),
          static_cast<char>(length & 0xFF)};
}

void ScopedPipe::SendSizeT(size_t size) const {
  SendBytes(SizeTToString(size));
}

size_t ScopedPipe::ReceiveSizeT() const {
  return StringToSizeT(ReceiveBytes(4));
}

void ScopedPipe::SendMessage(const std::string& message) const {
  SendSizeT(message.size());
  SendBytes(message);
}

std::string ScopedPipe::ReceiveMessage() const {
  size_t length = ReceiveSizeT();
  return ReceiveBytes(length);
}

const size_t kBufSize = 1024;

std::string ScopedPipe::ReceiveBytes(size_t count) const {
  static char buf[kBufSize];
  std::string bytes;
  while (count != 0) {
    std::size_t bytes_to_read = std::min(count, kBufSize);
    if (read(FdTo(), buf, bytes_to_read) < 0) {
      throw std::runtime_error(GetErrorMessage("failed to read bytes"));
    }
    count -= bytes_to_read;
    for (std::size_t i = 0; i < bytes_to_read; ++i) {
      bytes.push_back(buf[i]);
    }
  }
  return bytes;
}

void ScopedPipe::SendBytes(const std::string& bytes) const {
  size_t offset = 0;
  while (offset != bytes.size()) {
    auto res = write(FdFrom(), bytes.c_str() + offset, bytes.size() - offset);
    if (res < 0) {
      throw std::runtime_error(GetErrorMessage("failed to send bytes"));
    }
    offset += res;
  }
}

ScopedPipe::ScopedPipe(int* fd) {
  this->fd[0] = fd[0];
  this->fd[1] = fd[1];
}
