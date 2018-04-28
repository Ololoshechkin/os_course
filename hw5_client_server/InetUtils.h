//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETUTILS_H
#define HW5_CLIENT_SERVER_INETUTILS_H

#include <string>

struct InetUtils {
    static const std::string kLoopbackAddress;
    static const int kDefaultBacklogSize;
};

#endif //HW5_CLIENT_SERVER_INETUTILS_H
