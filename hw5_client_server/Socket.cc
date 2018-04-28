//
// Created by Vadim on 14/04/2018.
//

#include <iostream>
#include "Socket.h"

const size_t kBufSize = 1024;

Socket::Socket(int socket_fd) :
        SocketBase(socket_fd) {
//    std::cerr << "Socket() on fd=" << socket_fd << '\n';
}

Socket::Socket(InetSocketAddress const& address) :
        SocketBase() {
//    std::cerr << "Socket() on fd=" << socket_fd << '\n';
    auto system_address = address.ToSystemSocketAddress();
    if (connect(socket_fd, (struct sockaddr*) &system_address,
                sizeof(system_address)) < 0) {
        perror("connection failed");
        throw std::exception();
    }
}

std::string Socket::ReadPacket(size_t length) {
    char buf[kBufSize];
    std::string bytes;
    while (length != 0) {
        size_t bytes_to_read = std::min(length, kBufSize);
        if (recv(socket_fd, buf, bytes_to_read, 0) < 0) {
            perror("failed to read bytes");
            throw std::exception();
        }
        length -= bytes_to_read;
        for (size_t i = 0; i < bytes_to_read; ++i)
            bytes.push_back(buf[i]);
    }
    return bytes;
}

void Socket::WriteBytes(const std::string& bytes) {
    send(socket_fd, bytes.c_str(), bytes.size(), 0);
}

Socket::Socket() :
        SocketBase(-1) {
//    std::cerr << "~Socket() on fd=" << socket_fd << '\n';
}

int Socket::GetFd() const {
    return socket_fd;
}
