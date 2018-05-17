//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
#define HW5_CLIENT_SERVER_INETSOCKETADDRESS_H

#include <string>
#include <netinet/in.h>
#include "InetUtils.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

// Общие соображения про порядок инклюдов

using SystemSocketAddress = struct sockaddr_in;
// Ради чего? Чтобы только запутать? Представьте человека, которые умеет программирвать сокеты, зачем ему в этом
// месте лишний уровень абстракции?

class InetSocketAddress {
 public:
    int port;
    std::string host;
    // WTF? https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c9-minimize-exposure-of-members
    // Если есть класс - значит есть инвариант, если есть инвариант по членам данных - почему они публичные?

    explicit InetSocketAddress(
            int port, const std::string& host = InetUtils::kLoopbackAddress
    );
    // Зачем здесь explicit?

    SystemSocketAddress ToSystemSocketAddress() const;
};

// Кажется что эта абстракция излишняя. Инвариант не очевиден. Вполне достаточно свободных функций.

#endif //HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
