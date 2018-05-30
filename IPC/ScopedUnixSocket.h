//
// Created by Vadim on 28/05/2018.
//

#ifndef IPC_SCOPEDUNIXSOCKET_H
#define IPC_SCOPEDUNIXSOCKET_H

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

class ScopedUnixSocket {
 private:
  int socket_fd;
  struct sockaddr_un sockaddr_;
  explicit ScopedUnixSocket(int socket_fd);
 public:
  class TransferableFileDescriptorImage {
   private:
    struct msghdr msg;
    char buf[CMSG_SPACE(sizeof(int))];
    char dup[512];
    struct iovec io;
    template<typename Init>
    explicit TransferableFileDescriptorImage(Init init);
    
    friend class ScopedUnixSocket;
   
   public:
    ScopedUnixSocket ToUnixSocket() const;
    ~TransferableFileDescriptorImage();
  };
  
  ScopedUnixSocket();
  void Connect(const std::string& file_system_address);
  void BindAndListen(const std::string& file_system_address);
  ScopedUnixSocket Accept() const;
  void SendBytes(const std::string& bytes) const;
  std::string ReceiveBytes(size_t count) const;
  void SendSizeT(size_t size) const;
  size_t ReceiveSizeT() const;
  void SendMessage(const std::string& message) const;
  std::string ReceiveMessage() const;
  TransferableFileDescriptorImage ToImage() const;
  void SendFDImage(
          const TransferableFileDescriptorImage& image
  ) const;
  TransferableFileDescriptorImage ReceiveFDImage() const;
  ~ScopedUnixSocket();
};

#endif //IPC_SCOPEDUNIXSOCKET_H
