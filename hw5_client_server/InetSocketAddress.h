//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
#define HW5_CLIENT_SERVER_INETSOCKETADDRESS_H

#include <string>
#include <netinet/in.h>
#include "InetUtils.h"

class InetSocketAddress {
 public:
  explicit InetSocketAddress(
          int port, const std::string& host = InetUtils::kLoopbackAddress
  );
  sockaddr_in ToSystemSocketAddress() const;
 private:
    int port;
    std::string host;
};

#endif //HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
