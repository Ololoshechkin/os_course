//
// Created by Vadim on 14/04/2018.
//

#ifndef HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
#define HW5_CLIENT_SERVER_INETSOCKETADDRESS_H

#include <netinet/in.h>
#include <string>

class InetSocketAddress {
 public:
  static const std::string kLoopbackAddress;
  explicit InetSocketAddress(
          int port, const std::string& host = kLoopbackAddress
  );
  sockaddr_in ToSystemSocketAddress() const;
 private:
  int port;
  std::string host;
};

#endif //HW5_CLIENT_SERVER_INETSOCKETADDRESS_H
