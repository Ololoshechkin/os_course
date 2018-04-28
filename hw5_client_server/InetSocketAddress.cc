//
// Created by Vadim on 14/04/2018.
//

#include <libnet.h>
#include <utility>
#include "InetSocketAddress.h"

InetSocketAddress::InetSocketAddress(int port, std::string host) :
        port(port), host(std::move(host)) {}

SystemSocketAddress InetSocketAddress::ToSystemSocketAddress() const {
  SystemSocketAddress system_socket_address;
  system_socket_address.sin_family = AF_INET;
  system_socket_address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &system_socket_address.sin_addr);
  assert(system_socket_address.sin_addr.s_addr == htonl(INADDR_LOOPBACK));
  return system_socket_address;
}
