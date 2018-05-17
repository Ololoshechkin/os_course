//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_SOCKET_H
#define HW5_CLIENT_SERVER_SOCKET_H

#include "SocketBase.h"
#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// Обычно порядок инклюдов в заголовочных файлах такойЖ
// 1. Не портируемые интерфейсы и интерфейсы ОС
// 2. Портируемые интерфейсы и интерфейсы языка и библиотеки
// 3. Ваши собственные

class ServerSocket;

class ByteReadChanel;

class ByteWriteChanel;

// https://google.github.io/styleguide/cppguide.html#Forward_Declarations


class Socket : public SocketBase, public std::enable_shared_from_this<Socket> {
 private:
  explicit Socket(int socket_fd);
  friend ServerSocket;
  friend ByteReadChanel;
  friend ByteWriteChanel;
  // Чисто по интерфейсу возникает ощущение https://en.wikipedia.org/wiki/Code_smell
 public:
  explicit Socket();
  // Зачем здесь explicit?
  explicit Socket(InetSocketAddress const& address);
  std::string ReadPacket(size_t length);
  // size_t определяется в stddef.h или cstddef. Ни один не подключен. Почему используем?
  // Если используем cstddef - нужно std::size_t
  void WriteBytes(const std::string& bytes);
  int GetFd() const;
  ~Socket();
  // Хочется конструктор и деструктор видеть рядом
};

#endif //HW5_CLIENT_SERVER_SOCKET_H
