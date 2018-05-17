//
// Created by Vadim on 15/04/2018.
//

#include <sys/socket.h>
#include <cstdio>
#include <exception>
#include <iostream>
#include <unistd.h>
#include "SocketBase.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rs-consistency
// Зачем инклюдить одни и те же заголовки несколько раз?
// Помимо этого считаю хорошим тоном отделять инклюды разных типов - как минимум своего проекта от системных.
// При этом внутри группы должна быть сортировка по имени.

SocketBase::SocketBase() :
        SocketBase(socket(AF_INET, SOCK_STREAM, 0)) {
  if (socket_fd < 0) {
    perror("failed to open server_socket_");
    throw std::exception();
    // Тот кто будет перехватывать это исключение, захочет сделать e.what(). Что он увидит? Что скажет пустое сообщение тому, кто будет его читать?
    // Почему не засунуть strerror(errno) в это исключение?
  }
}

SocketBase::SocketBase(int socket_fd) :
        socket_fd(socket_fd) {
  // Кажется что в этом месте очень хорошо пришелся бы assert, что !(fd < 0)
}

SocketBase::~SocketBase() {
  close(socket_fd);
  // А вот здесь неплохо бы в stderr вывести ошибку, что произошла ошибка при закрытии сокета и показать что в errno
}

size_t SocketBase::Hash() {
  return static_cast<size_t>(socket_fd);
}
