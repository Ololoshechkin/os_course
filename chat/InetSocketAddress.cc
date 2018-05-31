//
// Created by Vadim on 14/04/2018.
//

#include <arpa/inet.h>

#include <utility>

#include "InetSocketAddress.h"

InetSocketAddress::InetSocketAddress(int port, const std::string& host) :
        port(port), host(host) {}
        
sockaddr_in InetSocketAddress::ToSystemSocketAddress() const {
  sockaddr_in system_socket_address;
  system_socket_address.sin_family = AF_INET;
  system_socket_address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &system_socket_address.sin_addr);
  return system_socket_address;
}
