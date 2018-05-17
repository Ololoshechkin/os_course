//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETUTILS_H
#define HW5_CLIENT_SERVER_INETUTILS_H

#include <string>
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// Зачем здесь все эти заголовочные файлы?

struct InetUtils {
  static const std::string kLoopbackAddress;
  static const int kDefaultBacklogSize;
};
#endif //HW5_CLIENT_SERVER_INETUTILS_H
