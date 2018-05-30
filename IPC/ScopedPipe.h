//
// Created by Vadim on 30/05/2018.
//

#ifndef IPC_SCOPEDPIPE_H
#define IPC_SCOPEDPIPE_H

#include <string>
#include "ScopedUnixSocket.h"

class ScopedUnixSocket;

class ScopedPipe {
 private:
  int fd[2];
  int FdFrom() const;
  int FdTo() const;
  explicit ScopedPipe(int fd[2]);
  
  friend class ScopedUnixSocket;
 
 public:
  ScopedPipe();
  ~ScopedPipe();
  void SendBytes(const std::string& bytes) const;
  std::string ReceiveBytes(size_t count) const;
  void SendSizeT(size_t size) const;
  size_t ReceiveSizeT() const;
  void SendMessage(const std::string& message) const;
  std::string ReceiveMessage() const;
};

#endif //IPC_SCOPEDPIPE_H
