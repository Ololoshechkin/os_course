//
// Created by Vadim on 26/05/2018.
//

#ifndef MULTIPLEXING_ASYNCCLIENT_H
#define MULTIPLEXING_ASYNCCLIENT_H

#include <string>
#include <vector>
#include "ScopedMultiplexer.h"
#include "Socket.h"

class AsyncClient {
 public:
  explicit AsyncClient(const InetSocketAddress& address);
  ~AsyncClient();
  void Start();
  static const std::string kExitMessage;
 private:
  ScopedMultiplexer multiplexer;
  Socket socket;
  std::vector<std::string> input_strings;
  size_t requests_in_porcess;
  std::string current_received_buffer;
  bool socket_is_subscribed;
  void ProcessBuffer();
  void CheckAndUpdateSubscriptions(
          const Event& event, const ScopedMultiplexer::Handler& events_handler
  );
};

#endif //MULTIPLEXING_ASYNCCLIENT_H
