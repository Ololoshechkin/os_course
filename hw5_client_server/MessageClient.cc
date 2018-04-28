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
        const std::string& name,
        std::function<void(const messages::Message&)> on_message_receive,
        std::function<void()> on_chat_disconnect,
        std::function<
                std::string(const std::vector<std::string>&)
        > on_chat_request
) :
        client_socket(std::make_shared<Socket>(server_address)), name(name),
        on_message_receive(std::move(on_message_receive)),
        on_chat_disconnect(std::move(on_chat_disconnect)),
        on_chat_request(std::move(on_chat_request)), state(AVAILABLE) {
    using namespace std::chrono_literals;
    messages::Register reg_message;
    std::cout << "name = " << name << '\n';
    reg_message.set_name(name);
    SendMessage(client_socket, reg_message, MessageType::REGISTER);
    auto session = ReceiveMessageAndType<messages::Session>(client_socket);
    session_id = session.session_id();
    listening_thread = std::thread([this] {
        auto update_period = 500ms;
        std::this_thread::sleep_for(update_period);
        while (true) {
            std::this_thread::sleep_for(update_period);
            update_lock_.lock();
            if (state == IN_CHAT) {
                UpdateMessages();
            } else {
                UpdateChatRequest();
            }
            update_lock_.unlock();
        }
    });
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
    auto in_chat = ReceiveMessageAndType<messages::CreateChatAcknolagement>(
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
                on_message_receive(
                        ReceiveMessage<messages::Message>(client_socket));
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
        state = GOT_CHAT_REQUESTS;
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



#pragma clang diagnostic pop