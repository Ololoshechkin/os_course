//
// Created by Vadim on 15/04/2018.
//

#include "MessageServer.h"
#include <utility>
#include "message.pb.h"
#include "session.pb.h"
#include <future>
#include <memory>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <mutex>
#include <algorithm>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
size_t (* MessageServer::socket_hasher)(std::shared_ptr<Socket>) = [](
      std::shared_ptr<Socket> socket
) {
  return socket->Hash();
};

template<typename T = std::string>
void debug_log(
  const std::shared_ptr<Socket>& socket,
  const std::string& log,
  T extra = ""
) {
  std::cerr << "      [" << socket->GetFd() << "] : " << log << " " << extra
            << std::endl;
}

MessageServer::MessageServer(const InetSocketAddress& server_address) :
  server_socket_(std::make_shared<ServerSocket>(server_address)),
  available_clients_(0, socket_hasher), chat_partner_(0, socket_hasher),
  client_to_name(0, socket_hasher), chat_requests_(0, socket_hasher),
  incoming_messages_(0, socket_hasher) {
}

void MessageServer::Run() {
  while (true) {
    std::cout << "attaching client\n";
    AttachClient(server_socket_->accept());
  }
}

void MessageServer::AttachClient(std::shared_ptr<Socket> client) {
  // TODO : use thread pool
  // TODO 2: no, it's only present in boost::
  client_sockets.push_back(client);
  client_threads.push_back(std::make_unique<std::thread>([this, client]() {
    std::cout << " starting client thread\n";
    std::cout << " registering client\n";
    while (!RegisterClient(client)) {
      debug_log(client, "failed to register");
    }
    try {
      bool client_is_alive = true;
      while (client_is_alive) {
        auto msg_type = ReceiveMessageType(client);
        switch (msg_type) {
        case EXIT:
          client_is_alive = UnregisterClient(client);
          break;
        case EXIT_CHAT:
          client_is_alive = ExitChatQuery(client);
          break;
        case GET_LIST_OF_CLIENTS:
          debug_log(client, "GET_LIST_OF_CLIENTS");
          client_is_alive = GetClientListQuery(client);
          break;
        case CREATE_CHAT:
          debug_log(client, "creating chat");
          client_is_alive = CreateChatQuery(client);
          break;
        case UPDATE_CHAT_REQUEST:
          client_is_alive = UpdateRequestsQuery(client);
          break;
        case MESSAGE:
          client_is_alive = SendMessageQuery(client);
          break;
        case UPDATE_MSGS:
          client_is_alive = UpdateMsgsQuery(client);
          break;
        default:
          break;
        }
      }
    } catch (const std::exception& e) {
      UnregisterClientImpl(client);
    }
  }));
}

bool MessageServer::RegisterClient(std::shared_ptr<Socket> client) {
  try {
    bool successful = false;
    auto register_msg = ReceiveMessageAndType<messages::Register>(client);
    session_lock_.lock();
    messages::Session session;
    if (name_to_client.count(register_msg.name())) {
      session.set_session_id(-1);
    } else {
      available_clients_[client] = register_msg;
      name_to_client[register_msg.name()] = client;
      client_to_name[client] = register_msg.name();
      session.set_session_id(first_free_session_++);
      successful = true;
    }
    session_lock_.unlock();
    SendMessage(client, session, MessageType::SESSION);
    debug_log(client, "\n\n\n\n\n");
    return successful;
  } catch (const std::exception& e) {
    return false;
  }
}

void MessageServer::UnregisterClientImpl(std::shared_ptr<Socket> client) {
  if (chat_partner_.count(client)) {
    ExitChatQuery(client);
  }
  name_to_client.erase(available_clients_[client].name());
  client_to_name.erase(client);
  available_clients_.erase(client);
}

bool MessageServer::UnregisterClient(std::shared_ptr<Socket> client) {
  session_lock_.lock();
  UnregisterClientImpl(std::move(client));
  session_lock_.unlock();
  return false;
}

bool MessageServer::GetClientListQuery(std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    auto session_id = ReceiveMessage<messages::GetListOfClients>(
      client).session_id();
    session_lock_.lock();
    debug_log(client, "session_lock_ locked");
    messages::ListOfClients reply;
    reply.set_session_id(session_id);
    for (auto it : available_clients_) {
      reply.add_client_names(it.second.name());
    }
    SendMessage(client, reply, MessageType::CLIENT_LIST);
    session_lock_.unlock();
    debug_log(client, "session_lock_ unlocked");
  });
}

bool MessageServer::ExitChatQuery(std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    debug_log(client, "exit chat");
    auto exit_chat = ReceiveMessage<messages::ExitChat>(client);
    auto partner = chat_partner_[client];
    debug_log(client, "exit chat, partner = ", partner);
    chat_partner_.erase(partner);
    chat_partner_.erase(client);
    debug_log(client, "exit chat, sending \"EXIT_CHAT\" to patner");
    SendMessage(partner, exit_chat, MessageType::EXIT_CHAT);
    debug_log(client, "exit chat, SENT");
  });
}

bool MessageServer::CreateChatQuery(std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    chat_lock_.lock();
    debug_log(client, "in creating chat");
    auto create_chat = ReceiveMessage<messages::CreateChat>(client);
    debug_log(client, "create_chat msg GOT");
    const auto& partner_name = create_chat.client_name();
    debug_log(client, "partner_name = " + partner_name);
    if (name_to_client.count(partner_name)) {
      auto partner = name_to_client[partner_name];
      chat_requests_[partner].push_back(client);
      debug_log(client, "out of creating chat");
    }
    chat_lock_.unlock();
  });
}

bool MessageServer::SendMessageQuery(const std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    debug_log(client, "send message query ");
    auto msg = ReceiveMessage<messages::Message>(client);
    chat_lock_.lock();
    debug_log(client, "lock acquired ");
    auto partner = chat_partner_[client];
    debug_log(client, "partner.fd : ", partner->GetFd());
    debug_log(client, "new messages count: ",
    incoming_messages_[partner].size());
    incoming_messages_[partner].push_back(msg.message_content());
    chat_lock_.unlock();
  });
}

bool MessageServer::UpdateRequestsQuery(std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    debug_log(client, "in UpdateRequestsQuery");
    ReceiveMessage<messages::UpdateChatRequest>(client);
    debug_log(client, "UpdateChatRequest got");
    chat_lock_.lock();
    messages::ChatRequests requests;
    requests.set_session_id(0);
    std::vector<std::string> requester_names;
    for (const auto& requester : chat_requests_[client]) {
      auto requester_name = client_to_name[requester];
      requests.add_partner_names(requester_name);
      requester_names.push_back(requester_name);
    }
    SendMessage(client, requests, MessageType::CHAT_REQUESTS);
    debug_log(client, "CHAT_REQUESTS sent (empty : ",
    requests.partner_names().empty());
    if (chat_requests_[client].empty()) {
      debug_log(client, "      empty-empty");
      chat_lock_.unlock();
      return;
    }
    debug_log(client, "      non-empty");
    auto type = ReceiveMessageType(client);
    debug_log(client, "      type : ", type);
    // now, server skips all old UPDATE_CHAT_REQUEST-messages
    while (type == MessageType::UPDATE_CHAT_REQUEST) {
      ReceiveMessage<messages::UpdateChatRequest>(client);
      type = ReceiveMessageType(client);
    }
    auto chosen_partner = ReceiveMessage<messages::ChosenPartner>(
      client).partner_name();
    debug_log(client, "      !chosen! : ", chosen_partner);
    for (const std::string& name : requester_names) {
      messages::CreateChatAcknolagement acknolagement;
      acknolagement.set_session_id(0);
      acknolagement.set_acknolaged(name == chosen_partner);
      SendMessage(name_to_client[name], acknolagement,
      MessageType::CREATE_CHAT_ACKNOLAGEMENT);
      if (acknolagement.acknolaged()) {
        chat_partner_[client] = name_to_client[name];
        chat_partner_[chat_partner_[client]] = client;
      }
    }
    chat_requests_[client].clear();
    chat_lock_.unlock();
  });
}

bool MessageServer::UpdateMsgsQuery(std::shared_ptr<Socket> client) {
  return TryActionOrDisconnect(client, [this, &client] {
    ReceiveMessage<messages::UpdateIncomingMessages>(client);
    chat_lock_.lock();
    while (!incoming_messages_[client].empty()) {
      auto msg_content = incoming_messages_[client].back();
      incoming_messages_[client].pop_back();
      messages::Message msg;
      msg.set_session_id(0);
      msg.set_message_content(msg_content);
      SendMessage(client, msg, MessageType::MESSAGE);
    }
    chat_lock_.unlock();
    messages::NoMessagesLeft teminate;
    teminate.set_session_id(0);
    SendMessage(client, teminate, MessageType::NO_MESSAGES_LEFT);
  });
}

MessageServer::~MessageServer() = default;
#pragma clang diagnostic pop