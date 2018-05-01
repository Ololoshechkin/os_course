//
// Created by Vadim on 15/04/2018.
//

#ifndef HW5_CLIENT_SERVER_MESSAGESERVER_H
#define HW5_CLIENT_SERVER_MESSAGESERVER_H

#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include "message.pb.h"
#include "session.pb.h"
#include "session.pb.h"
#include "message.pb.h"
#include "MessageUnit.h"
#include <memory.h>
#include <thread>
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

class MessageServer : public MessageUnit {
 public:
  explicit MessageServer(const InetSocketAddress& server_address);
  void Run();
  ~MessageServer();
 private:
  void AttachClient(std::shared_ptr<Socket> client);
  bool RegisterClient(std::shared_ptr<Socket> client);
  bool UnregisterClient(std::shared_ptr<Socket> client);
  void UnregisterClientImpl(std::shared_ptr<Socket> client);
  
  template<typename F>
  bool TryActionOrDisconnect(std::shared_ptr<Socket> client, F action) {
    try {
      action();
      return true;
    } catch (const std::exception& e) {
      UnregisterClientImpl(client);
      return false;
    }
  }
  
  bool GetClientListQuery(std::shared_ptr<Socket> client);
  bool ExitChatQuery(std::shared_ptr<Socket> client);
  bool CreateChatQuery(std::shared_ptr<Socket> client);
  bool UpdateRequestsQuery(std::shared_ptr<Socket> client);
  bool SendMessageQuery(std::shared_ptr<Socket> client);
  bool UpdateMsgsQuery(std::shared_ptr<Socket> client);
  std::shared_ptr<ServerSocket> server_socket_;
  std::mutex session_lock_;
  static size_t (* socket_hasher)(std::shared_ptr<Socket>);
  std::unordered_map<
          std::shared_ptr<Socket>, messages::Register,
          size_t(*)(std::shared_ptr<Socket>)
  > available_clients_;
  std::unordered_map<std::string, std::shared_ptr<Socket>> name_to_client;
  std::unordered_map<
          std::shared_ptr<Socket>, std::string,
          size_t(*)(std::shared_ptr<Socket>)
  > client_to_name;
  std::mutex chat_lock_;
  std::unordered_map<
          std::shared_ptr<Socket>, std::vector<std::shared_ptr<Socket>>,
          size_t(*)(std::shared_ptr<Socket>)
  > chat_requests_;
  std::unordered_map<
          std::shared_ptr<Socket>, std::vector<std::string>,
          size_t(*)(std::shared_ptr<Socket>)
  > incoming_messages_;
  std::unordered_map<
          std::shared_ptr<Socket>, std::shared_ptr<Socket>,
          size_t(*)(std::shared_ptr<Socket>)
  > chat_partner_;
  std::vector<std::unique_ptr<std::thread>> client_threads;
  std::vector<std::shared_ptr<Socket>> client_sockets;
  int32_t first_free_session_ = 0;
};

#endif //HW5_CLIENT_SERVER_MESSAGESERVER_H
