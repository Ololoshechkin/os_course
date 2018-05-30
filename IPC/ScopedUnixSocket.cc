//
// Created by Vadim on 28/05/2018.
//

#include <stdexcept>
#include <iostream>
#include <sys/un.h>
#include <sys/stat.h>
#include "ScopedUnixSocket.h"
#include "ExceptionHelp.h"

const size_t kBufSize = 1024;

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
  if (listen(socket_fd, 0) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to listen for new connections"));
  }
}

void ScopedUnixSocket::Connect(const std::string& file_system_address) {
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

ScopedUnixSocket
ScopedUnixSocket::TransferableFileDescriptorImage::ToUnixSocket() const {
  std::cout << "ToUnixSocket" << std::endl;
  cmsghdr* structcmsghdr;
  std::cout << "structcmsghdr ok" << std::endl;
  structcmsghdr = CMSG_FIRSTHDR(&msg);
  std::cout << "CMSG_FIRSTHDR ok" << std::endl;
  auto* data = (int*) CMSG_DATA(structcmsghdr);
  std::cout << "CMSG_DATA ok" << std::endl;
  int* int_data = (int*) (void*) data;
  std::cout << "int_data ok" << std::endl;
  int fd = int_data[0];
  std::cout << "memcpy ok" << std::endl;
  return ScopedUnixSocket(fd);
}

template<typename Init>
ScopedUnixSocket::TransferableFileDescriptorImage::TransferableFileDescriptorImage(
        Init init
) :
        msg({}) {
  init(msg, io, buf, dup);
}

ScopedUnixSocket::TransferableFileDescriptorImage::~TransferableFileDescriptorImage() = default;

ScopedUnixSocket::TransferableFileDescriptorImage
ScopedUnixSocket::ToImage() const {
  return TransferableFileDescriptorImage(
          [this](
                  struct msghdr& msg, struct iovec& io, char* buffer, char* dup
          ) {
            std::cout << "ToImage" << std::endl;
            std::cout << "buf ok, sizeof(buf) = " << sizeof(buffer)
                      << std::endl;
            memset(buffer, 0, sizeof(buffer));
            std::cout << "memset ok" << std::endl;
            io = iovec{.iov_base = (void*) "", .iov_len = 1};
            msg.msg_iov = &io;
            msg.msg_iovlen = 1;
            msg.msg_control = buffer;
            msg.msg_controllen = sizeof(buffer);
            std::cout << "msg ok" << std::endl;
            struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
            std::cout << "cmsghdr ok" << std::endl;
            cmsg->cmsg_level = SOL_SOCKET;
            std::cout << "SOL_SOCKET ok" << std::endl;
            cmsg->cmsg_type = SCM_RIGHTS;
            std::cout << "SCM_RIGHTS ok" << std::endl;
            cmsg->cmsg_len = CMSG_LEN(sizeof(socket_fd));
            std::cout << "cmsg ok" << std::endl;
            memmove(CMSG_DATA(cmsg), &socket_fd, sizeof(socket_fd));
            std::cout << "memmov ok" << std::endl;
            msg.msg_controllen = cmsg->cmsg_len;
          });
}

void ScopedUnixSocket::SendFDImage(
        const TransferableFileDescriptorImage& image
) const {
  std::cout << "SendFDImage" << std::endl;
  if (sendmsg(socket_fd, &image.msg, 0) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to send file descriptor"));
  }
  std::cout << "sent" << std::endl;
}

ScopedUnixSocket::TransferableFileDescriptorImage
ScopedUnixSocket::ReceiveFDImage() const {
  return TransferableFileDescriptorImage(
          [this](
                  struct msghdr& msg, struct iovec& io, char* buffer, char* dup
          ) {
            bzero(buffer, sizeof(buffer));
            std::cout << "bzero ok" << std::endl;
            msg.msg_control = buffer;
            std::cout << "msg_control ok" << std::endl;
            msg.msg_controllen = sizeof(buffer);
            std::cout << "msg_controllen ok" << std::endl;
            io = {dup, sizeof(dup)};
            std::cout << "io ok" << std::endl;
            msg.msg_iov = &io;
            std::cout << "io(2) ok" << std::endl;
            msg.msg_iovlen = 1;
            std::cout << "msg_iovlen ok" << std::endl;
            if (recvmsg(socket_fd, &msg, 0) < 0) {
              throw std::runtime_error(
                      GetErrorMessage("failed to receive file descriptor"));
            }
            std::cout << "received ok" << std::endl;
          });
}

std::size_t StringToSizeT(const std::string& s) {
  std::size_t int_val = 0;
  for (std::size_t i = 0; i < 4; ++i) {
    const auto cur = static_cast<std::size_t>(s[i]);
    int_val = (int_val << 8) | cur;
  }
  return int_val;
}

std::string SizeTToString(std::size_t length) {
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
      perror("failed to read bytes");
      throw std::exception();
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
