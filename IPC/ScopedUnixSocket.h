//
// Created by Vadim on 28/05/2018.
//

#ifndef IPC_SCOPEDUNIXSOCKET_H
#define IPC_SCOPEDUNIXSOCKET_H

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ScopedPipe.h"

class ScopedPipe;

class ScopedUnixSocket {
 private:
  int socket_fd;
  struct sockaddr_un sockaddr_;
 protected:
  explicit ScopedUnixSocket(int socket_fd);
 public:
  std::string address___todo_delete;
  int _det_fd__todo_delete() const;
  ScopedUnixSocket();
  ScopedUnixSocket(ScopedUnixSocket&& other) noexcept;
  ScopedUnixSocket(const ScopedUnixSocket& other) = delete;
  ScopedUnixSocket& operator=(ScopedUnixSocket&& other) noexcept;
  void Connect(const std::string& file_system_address);
  void BindAndListen(const std::string& file_system_address);
  ScopedUnixSocket Accept() const;
  void SendBytes(const std::string& bytes) const;
  std::string ReceiveBytes(size_t count) const;
  void SendSizeT(size_t size) const;
  size_t ReceiveSizeT() const;
  void SendMessage(const std::string& message) const;
  std::string ReceiveMessage() const;
  void SendFileDescriptor(int fd) const;
  int ReceiveFileDescriptor() const;
  void SendPipe(const ScopedPipe& socket) const;
  ScopedPipe ReceivePipe() const;
  ~ScopedUnixSocket();
};

#endif //IPC_SCOPEDUNIXSOCKET_H
