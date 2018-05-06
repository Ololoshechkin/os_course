//
// Created by Vadim on 14/04/2018.
//

#include <arpa/inet.h>
#include <utility>
#include "InetSocketAddress.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

InetSocketAddress::InetSocketAddress(int port, std::string host) :
        port(port), host(std::move(host)) {}

SystemSocketAddress InetSocketAddress::ToSystemSocketAddress() const {
  SystemSocketAddress system_socket_address;
  system_socket_address.sin_family = AF_INET;
  system_socket_address.sin_port = htons(port);
  inet_pton(AF_INET, host.c_str(), &system_socket_address.sin_addr);
  return system_socket_address;
}
