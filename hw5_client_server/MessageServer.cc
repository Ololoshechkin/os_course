//
// Created by Vadim on 15/04/2018.
//

#include "MessageServer.h"
#include <utility>
#include "message.pb.h"
#include "session.pb.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
size_t (* MessageServer::socket_hasher)(std::shared_ptr<Socket>) = [](
        std::shared_ptr<Socket> socket
) {
    return socket->Hash();
};

MessageServer::MessageServer(const InetSocketAddress& server_address) :
        server_socket_(std::make_shared<ServerSocket>(server_address)),
        available_clients_(0, socket_hasher), chat_partner_(0, socket_hasher),
        client_to_name(0, socket_hasher), chat_requests_(0, socket_hasher),
        incoming_messages_(0, socket_hasher) {
}

void MessageServer::Run() {
    std::cout << "starting thread\n";
//    auto run_thread = std::thread([this]() {
    std::cout << "in thread\n";
    while (true) {
        std::cout << "attaching client\n";
        AttachClient(server_socket_->accept());
    }
//    });
//    run_thread.detach();
}

void MessageServer::AttachClient(std::shared_ptr<Socket> client) {
    // TODO : use thread pool
    // TODO 2: no, it's only present in boost::
//    client_threads_lock_.lock();
    client_sockets.push_back(client);
    client_threads.push_back(std::make_unique<std::thread>([this, client]() {
        std::cout << " starting client thread\n";
        std::cout << " registering client\n";
        RegisterClient(client);
        try {
            while (true) {
                auto msg_type = ReceiveMessageType(client);
                switch (msg_type) {
                    case EXIT:
                        UnregisterClient(client);
                        break;
                    case EXIT_CHAT:
                        ExitChatQuery(client);
                        break;
                    case GET_LIST_OF_CLIENTS:
                        std::cout << "       " << client->GetFd()
                                  << "    GET_LIST_OF_CLIENTS\n";
                        GetClientListQuery(client);
                        break;
                    case CREATE_CHAT:
                        std::cout << "creating chat\n";
                        CreateChatQuery(client);
                        break;
                    case UPDATE_CHAT_REQUEST:
                        UpdateRequestsQuery(client);
                        break;
                    case MESSAGE:
                        SendMessageQuery(client);
                        break;
                    case UPDATE_MSGS:
                        UpdateMsgsQuery(client);
                        break;
                    default:
                        break;
                }
            }
        } catch (const std::exception& e) {
            UnregisterClientImpl(client);
        }
    }));
//    client_threads_lock_.unlock();
}

void MessageServer::RegisterClient(std::shared_ptr<Socket> client) {
    session_lock_.lock();
    auto register_msg = ReceiveMessageAndType<messages::Register>(client);
    available_clients_[client] = register_msg;
    name_to_client[register_msg.name()] = client;
    client_to_name[client] = register_msg.name();
    messages::Session session;
    session.set_session_id(first_free_session_++);
    session_lock_.unlock();
    SendMessage(client, session, MessageType::SESSION);
    std::cerr << "\n\n\n\n\n\n\n";
}

void MessageServer::UnregisterClientImpl(std::shared_ptr<Socket> client) {
    if (chat_partner_.count(client)) {
        ExitChatQuery(client);
    }
    name_to_client.erase(available_clients_[client].name());
    available_clients_.erase(client);
}

void MessageServer::UnregisterClient(std::shared_ptr<Socket> client) {
    session_lock_.lock();
    UnregisterClientImpl(std::move(client));
    session_lock_.unlock();
}

void MessageServer::GetClientListQuery(std::shared_ptr<Socket> client) {
    try {
        auto session_id = ReceiveMessage<messages::GetListOfClients>(
                client).session_id();
        session_lock_.lock();
        std::cout << "session_lock_ locked\n";
        messages::ListOfClients reply;
        reply.set_session_id(session_id);
        for (auto it : available_clients_) {
            reply.add_client_names(it.second.name());
        }
        SendMessage(client, reply, MessageType::CLIENT_LIST);
        session_lock_.unlock();
        std::cout << "session_lock_ unlocked\n";
    } catch (const std::exception& e) {
        UnregisterClientImpl(client);
    }
}

void MessageServer::ExitChatQuery(std::shared_ptr<Socket> client) {
    auto exit_chat = ReceiveMessage<messages::ExitChat>(client);
    auto partner = chat_partner_[client];
    chat_partner_.erase(partner);
    chat_partner_.erase(client);
    try {
        SendMessage(partner, exit_chat, MessageType::EXIT_CHAT);
    } catch (const std::exception& e) {
        UnregisterClientImpl(client);
    }
}

void MessageServer::CreateChatQuery(std::shared_ptr<Socket> client) {
    chat_lock_.lock();
    std::cout << "in creating chat\n";
    auto create_chat = ReceiveMessage<messages::CreateChat>(client);
    std::cout << "create_chat msg GOT\n";
    auto partner_name = create_chat.client_name();
    std::cout << "partner_name = " << partner_name << "\n";
    if (name_to_client.count(partner_name)) {
        auto partner = name_to_client[partner_name];
        chat_requests_[partner].push_back(client);
        std::cout << "out of creating chat\n";
    }
    chat_lock_.unlock();
}

void MessageServer::SendMessageQuery(const std::shared_ptr<Socket> client) {
    try {
        std::cerr << "send message query " << std::endl;
        auto msg = ReceiveMessage<messages::Message>(client);
        chat_lock_.lock();
        std::cerr << "lock acquired " << std::endl;
        auto partner = chat_partner_[client];
        std::cerr << "partner.fd : " << partner->GetFd() << std::endl;
        std::cerr << "new messages count: " << incoming_messages_[partner].size() << std::endl;
        incoming_messages_[partner].push_back(msg.message_content());
        chat_lock_.unlock();
    } catch (const std::exception& e) {
        std::cerr << "exception " << std::endl;
        UnregisterClientImpl(client);
    }
}

void MessageServer::UpdateRequestsQuery(std::shared_ptr<Socket> client) {
    std::cerr << "      in UpdateRequestsQuery\n";
    ReceiveMessage<messages::UpdateChatRequest>(client);
    std::cerr << "      UpdateChatRequest got\n";
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
    std::cerr << "      CHAT_REQUESTS sent (empty : "
              << requests.partner_names().empty() << ")\n";
    if (chat_requests_[client].empty()) {
        std::cerr << "      empty-empty\n";
        chat_lock_.unlock();
        return;
    }
//    std::cerr << "      non-empty\n";
    auto chosen_partner = ReceiveMessageAndType<messages::ChosenPartner>(
            client).partner_name();
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
}

void MessageServer::UpdateMsgsQuery(std::shared_ptr<Socket> client) {
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
}

MessageServer::~MessageServer() = default;
#pragma clang diagnostic pop