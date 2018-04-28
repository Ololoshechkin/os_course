//
// Created by Vadim on 22/04/2018.
//

#include "MessageClient.h"
#include <utility>
#include <chrono>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

MessageClient::MessageClient(
        const InetSocketAddress& server_address,
        std::function<void(const messages::Message&)> on_message_receive,
        std::function<void()> on_chat_disconnect,
        std::function<
                std::string(const std::vector<std::string>&)
        > on_chat_request
) :
        client_socket(std::make_shared<Socket>(server_address)),
        on_message_receive(std::move(on_message_receive)),
        on_chat_disconnect(std::move(on_chat_disconnect)),
        on_chat_request(std::move(on_chat_request)), state(AVAILABLE) {
}

//users
std::vector<std::string> MessageClient::GetUsers() {
  in_message_with_reply_lock_.lock();
  SendMessage(client_socket, NewMessage<messages::GetListOfClients>(),
              MessageType::GET_LIST_OF_CLIENTS);
  auto reply = ReceiveMessageAndType<messages::ListOfClients>(
          client_socket).client_names();
  std::vector<std::string> result(static_cast<unsigned long>(reply.size()));
  std::transform(reply.begin(), reply.end(), result.begin(),
                 [](const std::string name) {
                   return name;
                 });
  in_message_with_reply_lock_.unlock();
  return result;
}

bool MessageClient::StartChatWith(const std::string& name) {
  in_message_with_reply_lock_.lock();
  auto request = NewMessage<messages::CreateChat>();
  request.set_client_name(name);
  SendMessage(client_socket, request, MessageType::CREATE_CHAT);
  auto type = ReceiveMessageType(client_socket);
  auto in_chat = ReceiveMessage<messages::CreateChatAcknolagement>(
          client_socket).acknolaged();
  if (in_chat)
    state = IN_CHAT;
  in_message_with_reply_lock_.unlock();
  return in_chat;
}

bool MessageClient::SendChatMessage(const std::string& message) {
  in_message_with_reply_lock_.lock();
  auto msg = NewMessage<messages::Message>();
  msg.set_message_content(message);
  SendMessage(client_socket, msg, MessageType::MESSAGE);
  in_message_with_reply_lock_.unlock();
  return state == IN_CHAT;
}

void MessageClient::UpdateMessages() {
  in_message_with_reply_lock_.lock();
  SendMessage(client_socket, NewMessage<messages::UpdateIncomingMessages>(),
              MessageType::UPDATE_MSGS);
  bool have_more_messages = true;
  while (have_more_messages) {
    auto msg_type = ReceiveMessageType(client_socket);
    switch (msg_type) {
      case MESSAGE:
        on_message_receive(ReceiveMessage<messages::Message>(client_socket));
        break;
      case EXIT_CHAT:
        ReceiveMessage<messages::ExitChat>(client_socket);
        state = AVAILABLE;
        on_chat_disconnect();
        have_more_messages = false;
        break;
      case NO_MESSAGES_LEFT:
        ReceiveMessage<messages::NoMessagesLeft>(client_socket);
        have_more_messages = false;
        break;
      default:
        break;
    }
  }
  in_message_with_reply_lock_.unlock();
}

void MessageClient::EndChat() {
//    UpdateMessages();
  in_message_with_reply_lock_.lock();
  if (state == IN_CHAT) {
    SendMessage(client_socket, NewMessage<messages::ExitChat>(),
                MessageType::EXIT_CHAT);
  }
  state = AVAILABLE;
  in_message_with_reply_lock_.unlock();
}

void MessageClient::Disconnect() {
  if (state == IN_CHAT)
    EndChat();
  SendMessage(client_socket, NewMessage<messages::Disconnect>(),
              MessageType::DISCONNECT);
  state = DISCONNECTED;
}

void MessageClient::UpdateChatRequest() {
  in_message_with_reply_lock_.lock();
  messages::UpdateChatRequest msg;
  msg.set_session_id(session_id);
  SendMessage(client_socket, msg, MessageType::UPDATE_CHAT_REQUEST);
  auto requests = ReceiveMessageAndType<messages::ChatRequests>(
          client_socket).partner_names();
  if (!requests.empty()) {
    std::vector<std::string> names;
    std::for_each(requests.begin(), requests.end(),
                  [&names](const std::string& name) {
                    names.push_back(name);
                  });
//    auto dbg = NewMessage<messages::ChosenPartner>();
//    dbg.set_partner_name("a");
//    SendMessage(client_socket, dbg, MessageType::CHOSEN_PARTNER);
    auto chosen_partner = on_chat_request(names);
    messages::ChosenPartner partner_msg;
    partner_msg.set_session_id(session_id);
    partner_msg.set_partner_name(chosen_partner);
    for (const std::string& name : names) {
      if (name == chosen_partner) {
        state = IN_CHAT;
      }
    }
    SendMessage(client_socket, partner_msg, MessageType::CHOSEN_PARTNER);
  }
  in_message_with_reply_lock_.unlock();
}

void MessageClient::StopUpdates() {
  update_lock_.lock();
}

void MessageClient::ContinueUpdates() {
  update_lock_.unlock();
}

bool MessageClient::TryRegister(const std::string& name) {
  using namespace std::chrono_literals;
  messages::Register reg_message;
  reg_message.set_name(name);
  SendMessage(client_socket, reg_message, MessageType::REGISTER);
  auto session = ReceiveMessageAndType<messages::Session>(client_socket);
  session_id = session.session_id();
  if (session_id == -1) {
    return false;
  }
  listening_thread = std::thread([this] {
    auto update_period = 500ms;
    std::this_thread::sleep_for(update_period);
    while (true) {
      std::this_thread::sleep_for(update_period);
      update_lock_.lock();
      if (state == IN_CHAT) {
        try {
          UpdateMessages();
        } catch (const std::exception& e) {
          update_lock_.unlock();
          std::terminate();
        }
      } else if (state == AVAILABLE) {
        try {
          UpdateChatRequest();
        } catch (const std::exception& e) {
          update_lock_.unlock();
          std::terminate();
        }
      } else { // state == DISCONNECTED
        std::terminate();
      }
      update_lock_.unlock();
    }
  });
  listening_thread.detach();
  return true;
}

MessageClient::~MessageClient() = default;
#pragma clang diagnostic pop