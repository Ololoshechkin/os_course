//
// Created by Vadim on 15/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKETBASE_H
#define HW5_CLIENT_SERVER_SOCKETBASE_H

#include <memory.h>

class SocketBase {
 protected:
    int socket_fd;
    SocketBase();
    explicit SocketBase(int socket_fd);
    virtual ~SocketBase();
 public:
    virtual bool operator==(const SocketBase& other) const {
        return socket_fd == other.socket_fd;
    }
    size_t Hash();
};

#endif //HW5_CLIENT_SERVER_SOCKETBASE_H
