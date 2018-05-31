//
// Created by Vadim on 22/04/2018.
//

#ifndef HW5_CLIENT_SERVER_MESSAGECLIENT_H
#define HW5_CLIENT_SERVER_MESSAGECLIENT_H

#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>

#include "InetSocketAddress.h"
#include "session.pb.h"
#include "message.pb.h"
#include "Socket.h"
#include "MessageUtils.h"


template<typename UserMessageHandler>
class MessageClient {
 public:
  MessageClient(
          const InetSocketAddress&, UserMessageHandler* delegate
  );
  ~MessageClient() = default;
  bool TryRegister(const std::string& name);
  std::vector<std::string> GetUsers();
  bool StartChatWith(const std::string& name);
  bool SendChatMessage(const std::string& message);
  void UpdateMessages();
  void UpdateChatRequest();
  void EndChat();
  void Disconnect();
  void StopUpdates();
  void ContinueUpdates();
  ClientState state;
 private:
  template<typename Message>
  Message NewMessage() {
    Message msg;
    msg.set_session_id(session_id);
    return msg;
  }
  
  std::shared_ptr<Socket> client_socket;
  int session_id;
  UserMessageHandler* delegate;
  std::thread listening_thread;
  std::mutex in_message_with_reply_lock_;
  std::mutex update_lock_;
};

template<typename Delegate>
MessageClient<Delegate>::MessageClient(
        const InetSocketAddress& server_address, Delegate* delegate
) :
        client_socket(std::make_shared<Socket>(server_address)),
        delegate(delegate) {
}

template<typename Delegate>
std::vector<std::string> MessageClient<Delegate>::GetUsers() {
  in_message_with_reply_lock_.lock();
  SendMessage(client_socket, NewMessage<messages::GetListOfClients>(),
              MessageType::GET_LIST_OF_CLIENTS);
  const auto reply = ReceiveMessageAndType<messages::ListOfClients>(
          client_socket).client_names();
  std::vector<std::string> result(static_cast<unsigned long>(reply.size()));
  std::transform(reply.begin(), reply.end(), result.begin(),
                 [](const std::string name) {
                   return name;
                 });
  in_message_with_reply_lock_.unlock();
  return result;
}

template<typename Delegate>
bool MessageClient<Delegate>::StartChatWith(const std::string& name) {
  in_message_with_reply_lock_.lock();
  auto request = NewMessage<messages::CreateChat>();
  request.set_client_name(name);
  SendMessage(client_socket, request, MessageType::CREATE_CHAT);
  std::cout << "CREATE_CHAT - sent\n";
  const auto type = ReceiveMessageType(client_socket);
  std::cout << "TYPE : " << type << "\n";
  const auto in_chat = ReceiveMessage<messages::CreateChatAcknolagement>(
          client_socket).acknolaged();
  std::cout << "IN_CHAT : " << in_chat << "\n";
  if (in_chat)
    state = IN_CHAT;
  in_message_with_reply_lock_.unlock();
  return in_chat;
}

template<typename Delegate>
bool MessageClient<Delegate>::SendChatMessage(const std::string& message) {
  in_message_with_reply_lock_.lock();
  auto msg = NewMessage<messages::Message>();
  msg.set_message_content(message);
  SendMessage(client_socket, msg, MessageType::MESSAGE);
  in_message_with_reply_lock_.unlock();
  return state == IN_CHAT;
}

template<typename Delegate>
void MessageClient<Delegate>::UpdateMessages() {
  in_message_with_reply_lock_.lock();
  std::cout << "UpdateMessages\n";
  SendMessage(client_socket, NewMessage<messages::UpdateIncomingMessages>(),
              MessageType::UPDATE_MSGS);
  bool have_more_messages = true;
  while (have_more_messages) {
    const auto msg_type = ReceiveMessageType(client_socket);
    switch (msg_type) {
      case MESSAGE:
        delegate->on_message_receive(
                ReceiveMessage<messages::Message>(client_socket));
        break;
      case EXIT_CHAT:
        ReceiveMessage<messages::ExitChat>(client_socket);
        state = AVAILABLE;
        delegate->on_chat_disconnect();
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

template<typename Delegate>
void MessageClient<Delegate>::EndChat() {
//    UpdateMessages();
  in_message_with_reply_lock_.lock();
  if (state == IN_CHAT) {
    SendMessage(client_socket, NewMessage<messages::ExitChat>(),
                MessageType::EXIT_CHAT);
  }
  state = AVAILABLE;
  in_message_with_reply_lock_.unlock();
}

template<typename Delegate>
void MessageClient<Delegate>::Disconnect() {
  if (state == IN_CHAT)
    EndChat();
  SendMessage(client_socket, NewMessage<messages::Disconnect>(),
              MessageType::DISCONNECT);
  state = DISCONNECTED;
  listening_thread.join();
}

template<typename Delegate>
void MessageClient<Delegate>::UpdateChatRequest() {
  in_message_with_reply_lock_.lock();
  std::cout << "UpdateChatRequest\n";
  messages::UpdateChatRequest msg;
  msg.set_session_id(session_id);
  SendMessage(client_socket, msg, MessageType::UPDATE_CHAT_REQUEST);
  const auto requests = ReceiveMessageAndType<messages::ChatRequests>(
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
    const auto chosen_partner = delegate->on_chat_request(names);
    auto partner_msg = NewMessage<messages::ChosenPartner>();
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

template<typename Delegate>
void MessageClient<Delegate>::StopUpdates() {
  update_lock_.lock();
}

template<typename Delegate>
void MessageClient<Delegate>::ContinueUpdates() {
  update_lock_.unlock();
}

template<typename Delegate>
bool MessageClient<Delegate>::TryRegister(const std::string& name) {
  using namespace std::chrono_literals;
  messages::Register reg_message;
  reg_message.set_name(name);
  SendMessage(client_socket, reg_message, MessageType::REGISTER);
  const auto session = ReceiveMessageAndType<messages::Session>(client_socket);
  session_id = session.session_id();
  if (session_id == -1) {
    return false;
  }
  listening_thread = std::thread([this] {
    const auto update_period = 500ms;
    std::this_thread::sleep_for(update_period);
    while (true) {
      std::this_thread::sleep_for(update_period);
      update_lock_.lock();
      if (state == IN_CHAT) {
        try {
          UpdateMessages();
        } catch (const std::exception& e) {
          std::terminate();
        }
      } else if (state == AVAILABLE) {
        try {
          UpdateChatRequest();
        } catch (const std::exception& e) {
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

#endif //HW5_CLIENT_SERVER_MESSAGECLIENT_H
