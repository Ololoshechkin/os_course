//
// Created by Vadim on 28/05/2018.
//

#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include "ScopedUnixSocket.h"
#include "ExceptionHelp.h"

const size_t kBufSize = 1024;
const size_t kBacklogSize = 28;

ScopedUnixSocket::ScopedUnixSocket(int socket_fd) :
        socket_fd(socket_fd) {
  if (socket_fd < 0) {
    throw std::runtime_error(GetErrorMessage("failed to create Unix socket"));
  }
}

ScopedUnixSocket::ScopedUnixSocket() :
        ScopedUnixSocket(socket(AF_UNIX, SOCK_STREAM, 0)) {
}

ScopedUnixSocket::~ScopedUnixSocket() {
  if (socket_fd > 0 && close(socket_fd) < 0) {
    std::cout << GetErrorMessage("failed to close unix socket") << std::endl;
  }
}

void ScopedUnixSocket::BindAndListen(const std::string& file_system_address) {
  address___todo_delete = file_system_address;
  const char* file_path = (file_system_address + (char) 0).c_str();
  sockaddr_.sun_family = AF_UNIX;
  strcpy(sockaddr_.sun_path, file_path);
  struct stat stat_s{};
  if (stat(file_path, &stat_s) >= 0 && unlink(file_path) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to unlink file"));
  }
  if (bind(
          socket_fd, (struct sockaddr*) &sockaddr_, sizeof(sockaddr_)) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to bind to the given filepath"));
  }
  if (listen(socket_fd, kBacklogSize) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to listen for new connections"));
  }
}

void ScopedUnixSocket::Connect(const std::string& file_system_address) {
  address___todo_delete = file_system_address;
  sockaddr_.sun_family = AF_UNIX;
  const char* file_path = (file_system_address + (char) 0).c_str();
  strcpy(sockaddr_.sun_path, file_path);
  if (connect(
          socket_fd, (struct sockaddr*) &sockaddr_, sizeof(sockaddr_)) < 0) {
    throw std::runtime_error(
            GetErrorMessage(
                    "failed to connect to a unconnectedix socket with given filepath"));
  }
}

ScopedUnixSocket ScopedUnixSocket::Accept() const {
  socklen_t length = sizeof(sockaddr_);
  return ScopedUnixSocket(
          accept(
                  socket_fd, (struct sockaddr*) &sockaddr_, &length));
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

void ScopedUnixSocket::SendSizeT(size_t size) const {
  SendBytes(SizeTToString(size));
}

size_t ScopedUnixSocket::ReceiveSizeT() const {
  return StringToSizeT(ReceiveBytes(4));
}

void ScopedUnixSocket::SendMessage(const std::string& message) const {
  SendSizeT(message.size());
  SendBytes(message);
}

std::string ScopedUnixSocket::ReceiveMessage() const {
  size_t length = ReceiveSizeT();
  return ReceiveBytes(length);
}

std::string ScopedUnixSocket::ReceiveBytes(size_t count) const {
  static char buf[kBufSize];
  std::string bytes;
  while (count != 0) {
    std::size_t bytes_to_read = std::min(count, kBufSize);
    if (recv(socket_fd, buf, bytes_to_read, 0) < 0) {
      throw std::runtime_error(GetErrorMessage("failed to read bytes"));
    }
    count -= bytes_to_read;
    for (std::size_t i = 0; i < bytes_to_read; ++i) {
      bytes.push_back(buf[i]);
    }
  }
  return bytes;
}

void ScopedUnixSocket::SendBytes(const std::string& bytes) const {
  send(socket_fd, bytes.c_str(), bytes.size(), 0);
}

void ScopedUnixSocket::SendFileDescriptor(int fd) const {
  char buffer[CMSG_SPACE(sizeof(fd))];
  memset(buffer, 0, sizeof(buffer));
  iovec io{.iov_base = (void*) "", .iov_len = 1};
  struct msghdr msg = {nullptr};
  msg.msg_iov = &io;
  msg.msg_iovlen = 1;
  msg.msg_control = buffer;
  msg.msg_controllen = sizeof(buffer);
  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
  memmove(CMSG_DATA(cmsg), &fd, sizeof(fd));
  msg.msg_controllen = cmsg->cmsg_len;
  if (sendmsg(socket_fd, &msg, 0) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to send file descriptor"));
  }
}

int ScopedUnixSocket::ReceiveFileDescriptor() const {
  msghdr msg = {nullptr};
  cmsghdr* structcmsghdr;
  char buffer[CMSG_SPACE(sizeof(int))];
  char duplicate[512];
  bzero(buffer, sizeof(buffer));
  iovec io = {.iov_base = &duplicate, .iov_len = sizeof(duplicate)};
  msg.msg_control = buffer;
  msg.msg_controllen = sizeof(buffer);
  msg.msg_iov = &io;
  msg.msg_iovlen = 1;
  if (recvmsg(socket_fd, &msg, 0) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to receive file descriptor"));
  }
  structcmsghdr = CMSG_FIRSTHDR(&msg);
  int received_fd;
  memcpy(&received_fd, (int*) CMSG_DATA(structcmsghdr), sizeof(int));
  return received_fd;
}

int ScopedUnixSocket::_det_fd__todo_delete() const {
  return socket_fd;
}

void ScopedUnixSocket::SendPipe(const ScopedPipe& pipe) const {
  SendFileDescriptor(pipe.fd[0]);
  SendFileDescriptor(pipe.fd[1]);
}

ScopedPipe ScopedUnixSocket::ReceivePipe() const {
  int fd[2] = {ReceiveFileDescriptor(), ReceiveFileDescriptor()};
  return ScopedPipe(fd);
}

ScopedUnixSocket::ScopedUnixSocket(ScopedUnixSocket&& other) noexcept :
        socket_fd(other.socket_fd), sockaddr_(other.sockaddr_) {
  other.socket_fd = -1;
}

ScopedUnixSocket&
ScopedUnixSocket::operator=(ScopedUnixSocket&& other) noexcept {
  socket_fd = other.socket_fd;
  sockaddr_ = other.sockaddr_;
  other.socket_fd = -1;
  return *this;
}
