//
// Created by Vadim on 15/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKETBASE_H
#define HW5_CLIENT_SERVER_SOCKETBASE_H

#include <memory.h>
// Что это за заголовочный файл?

#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

class SocketBase {
 protected:
  int socket_fd;
  // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-protected

  SocketBase();
  explicit SocketBase(int socket_fd);
  virtual ~SocketBase();
 public:

  virtual bool operator==(const SocketBase& other) const {
    return socket_fd == other.socket_fd;
  }
  // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c87-beware-of--on-base-classes

  size_t Hash();
  // Почему не перегрузка std::hash? SA: http://en.cppreference.com/w/cpp/utility/hash
  // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c89-make-a-hash-noexcept
};

// Вообще по дизайну класса - не очевидно, какую роль он выполняет. Если это RAII контейнер - хотелось
// бы отразить это в названии - какой-то SocketScopedBase или что-то подобное.
// Т.е. на мой взгляд абстракция SocketBase не полноценна.


#endif //HW5_CLIENT_SERVER_SOCKETBASE_H
