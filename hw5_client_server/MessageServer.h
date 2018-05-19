//
// Created by Vadim on 15/04/2018.
//

#ifndef HW5_CLIENT_SERVER_MESSAGESERVER_H
#define HW5_CLIENT_SERVER_MESSAGESERVER_H

#include <cstddef>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "InetSocketAddress.h"
#include "ServerSocket.h"
#include "message.pb.h"
#include "session.pb.h"
#include "MessageUtils.h"


class MessageServer {
 public:
  explicit MessageServer(const InetSocketAddress& server_address);
  ~MessageServer();
  void Run();
 private:
  class ClientView {
   public:
    std::shared_ptr<Socket> client_socket;
    int32_t session_id;
    ClientView(
            std::shared_ptr<Socket> client_socket = std::make_shared<
                    Socket
            >(), int32_t session_id = -1
    );
    ClientView(const ClientView& other);
    ClientView& operator=(ClientView other);
    bool operator==(const ClientView& other) const;
    operator std::shared_ptr<Socket>() const;
   private:
    void Swap(ClientView& other);
  };
  
  struct ClientViewHash {
    std::size_t operator()(const ClientView& client) const noexcept {
      return std::hash<int32_t>{}(client.session_id);
    }
  };
  
  template<typename T = std::string>
  void debug_log(
          const MessageServer::ClientView& client,
          const std::string& log,
          T extra = ""
  );
  void AttachClient(std::shared_ptr<Socket> client);
  int32_t RegisterClient(std::shared_ptr<Socket> client);
  bool UnregisterClient(ClientView client);
  void UnregisterClientImpl(ClientView client);
  bool GetClientListQuery(ClientView client);
  bool ExitChatQuery(ClientView client);
  bool CreateChatQuery(ClientView client);
  bool UpdateRequestsQuery(ClientView client);
  bool SendMessageQuery(ClientView client);
  bool UpdateMsgsQuery(ClientView client);
  
  template<typename F>
  bool TryActionOrDisconnect(const ClientView& client, F action) {
    try {
      action();
      return true;
    } catch (const std::exception& e) {
      UnregisterClientImpl(client);
      return false;
    }
  }
  
  std::shared_ptr<ServerSocket> server_socket_;
  std::mutex session_lock_;
  std::mutex chat_lock_;
  std::unordered_map<
          ClientView, messages::Register, ClientViewHash
  > available_clients_;
  // std::unordered_map<int32_t, ClientView> session_to_client_;
  std::unordered_map<std::string, ClientView> name_to_client_;
  std::unordered_map<ClientView, std::string, ClientViewHash> client_to_name_;
  std::unordered_map<
          ClientView, std::vector<ClientView>, ClientViewHash
  > chat_requests_;
  std::unordered_map<
          ClientView, std::vector<std::string>, ClientViewHash
  > incoming_messages_;
  std::unordered_map<ClientView, ClientView, ClientViewHash> chat_partner_;
  std::vector<std::unique_ptr<std::thread>> client_threads;
  std::vector<std::shared_ptr<Socket>> client_sockets;
  int32_t first_free_session_;
};

#endif //HW5_CLIENT_SERVER_MESSAGESERVER_H
