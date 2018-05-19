// Created by Vadim on 22/04/2018.


#ifndef HW5_CLIENT_SERVER_MESSAGEUNIT_H
#define HW5_CLIENT_SERVER_MESSAGEUNIT_H

#include <cstddef>
#include <memory>
#include <unordered_map>

#include "Socket.h"


std::size_t StringToSizeT(const std::string& s);
std::string SizeTBytes(std::size_t length);

enum ClientState {
	AVAILABLE, IN_CHAT, DISCONNECTED
};
enum MessageType {
	REGISTER,
	EXIT,
	SESSION,
	MESSAGE,
	GET_LIST_OF_CLIENTS,
	CREATE_CHAT,
	EXIT_CHAT,
	CREATE_CHAT_ACKNOLAGEMENT,
	KEEP_ALIVE_REQUEST,
	KEEP_ALIVE_ANSWER,
	NO_MESSAGES_LEFT,
	DISCONNECT,
	CLIENT_LIST,
	UPDATE_CHAT_REQUEST,
	CHAT_REQUESTS,
	CHOSEN_PARTNER,
	UPDATE_MSGS
};

template<typename Message>
Message ReceiveMessage(const std::shared_ptr<Socket>& client) {
	const auto length_bytes = client->ReadPacket(4);
	const auto length = StringToSizeT(length_bytes);
	const auto msg_bytes = client->ReadPacket(length);
	Message message;
	message.ParseFromString(msg_bytes);
	return message;
}

MessageType ReceiveMessageType(const std::shared_ptr<Socket>& client);

template<typename Message>
Message ReceiveMessageAndType(const std::shared_ptr<Socket>& client) {
	ReceiveMessageType(client);
	return ReceiveMessage<Message>(client);
}

void
SendMessageType(const std::shared_ptr<Socket>& client, MessageType msg_type);

template<typename Message>
void SendMessage(
    const std::shared_ptr<Socket>& client,
    const Message& message,
    MessageType message_type
) {
	const auto serialized_msg = message.SerializeAsString();
	const auto length = serialized_msg.size();
	SendMessageType(client, message_type);
	client->WriteBytes(SizeTBytes(length));
	client->WriteBytes(serialized_msg);
}

#endif //HW5_CLIENT_SERVER_MESSAGEUNIT_H
