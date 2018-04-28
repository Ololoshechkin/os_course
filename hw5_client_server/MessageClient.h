//
// Created by Vadim on 22/04/2018.
//

#ifndef HW5_CLIENT_SERVER_MESSAGECLIENT_H
#define HW5_CLIENT_SERVER_MESSAGECLIENT_H

#include <thread>
#include "InetSocketAddress.h"
#include "Socket.h"
#include "MessageUnit.h"
#include "session.pb.h"
#include "message.pb.h"

class MessageClient : public MessageUnit {
 public:
    MessageClient(
            const InetSocketAddress&,
            const std::string& name,
            std::function<void(const messages::Message&)> on_message_receive,
            std::function<void()> on_chat_disconnect,
            std::function<std::string(const std::vector<std::string>&)> on_chat_request
    );
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
    std::string name;
    int session_id;
    std::function<void(const messages::Message&)> on_message_receive;
    std::function<void()> on_chat_disconnect;
    std::function<std::string(const std::vector<std::string>&)> on_chat_request;
    std::thread listening_thread;
    std::mutex in_message_with_reply_lock_;
    std::mutex update_lock_;
};

#endif //HW5_CLIENT_SERVER_MESSAGECLIENT_H
