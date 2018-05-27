//
// Created by Vadim on 26/05/2018.
//

#ifndef MULTIPLEXING_KOTLINNATIVESERVER_H
#define MULTIPLEXING_KOTLINNATIVESERVER_H

#include <unordered_map>
#include "ScopedMultiplexer.h"
#include "ServerSocket.h"

class KotlinNativeServer {
 public:
  explicit KotlinNativeServer(const InetSocketAddress& address);
  ~KotlinNativeServer();
  void Run();
  static const char kQueryEnd;
 private:
  ScopedMultiplexer multiplexer;
  ServerSocket server_socket;
  using Handler = ScopedMultiplexer::Handler;
  using Action = void (*)();
  Handler GetClientEventHandler(
          std::shared_ptr<Socket> client
  );
  std::unordered_map<int, std::string> fd_to_receive_buffer;
  std::unordered_map<int, std::string> fd_to_send_buffer;
  void CheckAndChangeSubscription(
          const std::string& received_bytes, std::shared_ptr<Socket> client,
          const Handler& client_events_handler
  );
  std::string ProcessRequest(std::string&& request);
};

#endif //MULTIPLEXING_KOTLINNATIVESERVER_H
