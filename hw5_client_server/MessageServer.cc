//
// Created by Vadim on 15/04/2018.
//

#include <iostream>

#include "MessageServer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

template<typename T>
void MessageServer::debug_log(
        const MessageServer::ClientView& client, const std::string& log, T extra
) {
  std::cerr << "      [" << client.session_id << "] : " << log << " " << extra
            << std::endl;
}

MessageServer::MessageServer(const InetSocketAddress& server_address) :
        server_socket_(std::make_shared<ServerSocket>(server_address)),
        first_free_session_(0) {
}

MessageServer::~MessageServer() = default;

void MessageServer::Run() {
  while (true) {
    std::cout << "attaching client\n";
    AttachClient(server_socket_->accept());
  }
}

MessageServer::ClientView::ClientView(
        std::shared_ptr<Socket> client_socket, int32_t session_id
) :
        client_socket(client_socket), session_id(session_id) {
}

MessageServer::ClientView::ClientView(const MessageServer::ClientView& other) :
        client_socket(other.client_socket), session_id(other.session_id) {
}

MessageServer::ClientView& MessageServer::ClientView::operator=(
        MessageServer::ClientView other
) {
  Swap(other);
  return *this;
}

void MessageServer::ClientView::Swap(MessageServer::ClientView& other) {
  std::swap(client_socket, other.client_socket);
  std::swap(session_id, other.session_id);
}

bool
MessageServer::ClientView::operator==(const MessageServer::ClientView& other) const {
  return session_id == other.session_id;
}

MessageServer::ClientView::operator std::shared_ptr<Socket>() const {
  return client_socket;
}

void MessageServer::AttachClient(std::shared_ptr<Socket> client_socket) {
  // TODO : use thread pool
  // TODO 2: no, it's only present in boost::
  client_sockets.push_back(client_socket);
  client_threads.push_back(
          std::make_unique<std::thread>([this, client_socket]() {
            std::cout << " starting client thread\n";
            std::cout << " registering client\n";
            int32_t session_id = -1;
            while (session_id == -1) {
              session_id = RegisterClient(client_socket);
            }
            auto client = ClientView(client_socket, session_id);
            try {
              bool client_is_alive = true;
              while (client_is_alive) {
                const auto msg_type = ReceiveMessageType(client_socket);
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

int32_t MessageServer::RegisterClient(std::shared_ptr<Socket> client) {
  int32_t result = -1;
  try {
    const auto register_msg = ReceiveMessageAndType<messages::Register>(client);
    session_lock_.lock();
    messages::Session session;
    if (name_to_client_.count(register_msg.name())) {
      session.set_session_id(-1);
    } else {
      const auto client_session = first_free_session_++;
      ClientView client_view(client, client_session);
      available_clients_[client_view] = register_msg;
      name_to_client_[register_msg.name()] = client_view;
      client_to_name_[client_view] = register_msg.name();
      session.set_session_id(client_session);
      result = client_session;
    }
    session_lock_.unlock();
    SendMessage(client, session, MessageType::SESSION);
    return result;
  } catch (const std::exception& e) {
    return result;
  }
}

void MessageServer::UnregisterClientImpl(MessageServer::ClientView client) {
  if (chat_partner_.count(client)) {
    ExitChatQuery(client);
  }
  name_to_client_.erase(available_clients_[client].name());
  client_to_name_.erase(client);
  available_clients_.erase(client);
}

bool MessageServer::UnregisterClient(MessageServer::ClientView client_session) {
  session_lock_.lock();
  UnregisterClientImpl(client_session);
  session_lock_.unlock();
  return false;
}

bool MessageServer::GetClientListQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    const auto session_id = ReceiveMessage<messages::GetListOfClients>(
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

bool MessageServer::ExitChatQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    chat_lock_.lock();
    debug_log(client, "exit chat");
    const auto exit_chat = ReceiveMessage<messages::ExitChat>(client);
    const auto partner = chat_partner_[client];
    debug_log(client, "exit chat, partner = ", partner.session_id);
    chat_partner_.erase(partner);
    chat_partner_.erase(client);
    debug_log(client, "exit chat, sending \"EXIT_CHAT\" to patner");
    SendMessage(partner, exit_chat, MessageType::EXIT_CHAT);
    debug_log(client, "exit chat, SENT");
    chat_lock_.unlock();
  });
}

bool MessageServer::CreateChatQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    chat_lock_.lock();
    debug_log(client, "in creating chat");
    const auto create_chat = ReceiveMessage<messages::CreateChat>(client);
    debug_log(client, "create_chat msg GOT");
    const auto& partner_name = create_chat.client_name();
    debug_log(client, "partner_name = " + partner_name);
    if (name_to_client_.count(partner_name)) {
      const auto partner = name_to_client_[partner_name];
      chat_requests_[partner].push_back(client);
      debug_log(client, "out of creating chat");
    }
    chat_lock_.unlock();
  });
}

bool MessageServer::SendMessageQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    debug_log(client, "send message query ");
    const auto msg = ReceiveMessage<messages::Message>(client);
    chat_lock_.lock();
    debug_log(client, "lock acquired ");
    const auto partner = chat_partner_[client];
    debug_log(client, "partner.fd : ", partner.session_id);
    debug_log(client, "new messages count: ",
              incoming_messages_[partner].size());
    incoming_messages_[partner].push_back(msg.message_content());
    chat_lock_.unlock();
  });
}

bool MessageServer::UpdateRequestsQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    debug_log(client, "in UpdateRequestsQuery");
    ReceiveMessage<messages::UpdateChatRequest>(client);
    debug_log(client, "UpdateChatRequest got");
    chat_lock_.lock();
    messages::ChatRequests requests;
    requests.set_session_id(0);
    std::vector<std::string> requester_names;
    for (const auto& requester : chat_requests_[client]) {
      const auto requester_name = client_to_name_[requester];
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
    // now, server skips all old UPDATE_CHAT_REQUEST-messages
    const auto type = ReceiveMessageType(client);
    debug_log(client, " message type : ", type);
    const auto chosen_partner = ReceiveMessage<messages::ChosenPartner>(
            client).partner_name();
    debug_log(client, "      !chosen! : ", chosen_partner);
    for (const std::string& name : requester_names) {
      messages::CreateChatAcknolagement acknolagement;
      acknolagement.set_session_id(0);
      acknolagement.set_acknolaged(name == chosen_partner);
      SendMessage(name_to_client_[name], acknolagement,
                  MessageType::CREATE_CHAT_ACKNOLAGEMENT);
      if (acknolagement.acknolaged()) {
        chat_partner_[client] = name_to_client_[name];
        chat_partner_[chat_partner_[client]] = client;
      }
    }
    // chosen_partner_.erase(client);
    chat_requests_[client].clear();
    chat_lock_.unlock();
  });
}

bool MessageServer::UpdateMsgsQuery(MessageServer::ClientView client) {
  return TryActionOrDisconnect(client, [this, &client] {
    ReceiveMessage<messages::UpdateIncomingMessages>(client);
    chat_lock_.lock();
    while (!incoming_messages_[client].empty()) {
      const auto msg_content = incoming_messages_[client].back();
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

#pragma clang diagnostic pop