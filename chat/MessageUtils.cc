//
// Created by Vadim on 22/04/2018.
//

#include <cstddef>
#include <sstream>

#include "MessageUtils.h"

std::size_t StringToSizeT(const std::string& s) {
  std::size_t int_val = 0;
  for (std::size_t i = 0; i < 4; ++i) {
    const auto cur = static_cast<std::size_t>(s[i]);
    int_val = (int_val << 8) | cur;
  }
  return int_val;
}

std::string SizeTBytes(std::size_t length) {
  return {static_cast<char>((length >> 24) & 0xFF),
          static_cast<char>((length >> 16) & 0xFF),
          static_cast<char>((length >> 8) & 0xFF),
          static_cast<char>(length & 0xFF)};
}

MessageType ReceiveMessageType(const std::shared_ptr<Socket>& client) {
	MessageType msg_type;
	const auto pack1 = client->ReadPacket(1);
	const auto msg_type_encoded = (int) pack1[0];
	switch (msg_type_encoded) {
	case 0:
		msg_type = REGISTER;
		break;
	case 1:
		msg_type = MESSAGE;
		break;
	case 2:
		msg_type = EXIT;
		break;
	case 3:
		msg_type = SESSION;
		break;
	case 4:
		msg_type = GET_LIST_OF_CLIENTS;
		break;
	case 5:
		msg_type = CREATE_CHAT;
		break;
	case 6:
		msg_type = EXIT_CHAT;
		break;
	case 7:
		msg_type = CREATE_CHAT_ACKNOLAGEMENT;
		break;
	case 8:
		msg_type = KEEP_ALIVE_REQUEST;
		break;
	case 9:
		msg_type = KEEP_ALIVE_ANSWER;
		break;
	case 10:
		msg_type = NO_MESSAGES_LEFT;
		break;
	case 11:
		msg_type = DISCONNECT;
		break;
	case 12:
		msg_type = CLIENT_LIST;
		break;
	case 13:
		msg_type = UPDATE_CHAT_REQUEST;
		break;
	case 14:
		msg_type = CHAT_REQUESTS;
		break;
	case 15:
		msg_type = CHOSEN_PARTNER;
		break;
	case 16:
		msg_type = UPDATE_MSGS;
		break;
	default:
		msg_type = DISCONNECT;
		break;
	}
	return msg_type;
}

void
SendMessageType(const std::shared_ptr<Socket>& client, MessageType msg_type) {
	int msg_type_code;
	switch (msg_type) {
	case REGISTER:
		msg_type_code = 0;
		break;
	case MESSAGE:
		msg_type_code = 1;
		break;
	case EXIT:
		msg_type_code = 2;
		break;
	case SESSION:
		msg_type_code = 3;
		break;
	case GET_LIST_OF_CLIENTS:
		msg_type_code = 4;
		break;
	case CREATE_CHAT:
		msg_type_code = 5;
		break;
	case EXIT_CHAT:
		msg_type_code = 6;
		break;
	case CREATE_CHAT_ACKNOLAGEMENT:
		msg_type_code = 7;
		break;
	case KEEP_ALIVE_REQUEST:
		msg_type_code = 8;
		break;
	case KEEP_ALIVE_ANSWER:
		msg_type_code = 9;
		break;
	case NO_MESSAGES_LEFT:
		msg_type_code = 10;
		break;
	case DISCONNECT:
		msg_type_code = 11;
		break;
	case CLIENT_LIST:
		msg_type_code = 12;
		break;
	case UPDATE_CHAT_REQUEST:
		msg_type_code = 13;
		break;
	case CHAT_REQUESTS:
		msg_type_code = 14;
		break;
	case CHOSEN_PARTNER:
		msg_type_code = 15;
		break;
	case UPDATE_MSGS:
		msg_type_code = 16;
		break;
	default:
		msg_type_code = 11;
		break;
	}
	std::string type_string{(char) msg_type_code};
	client->WriteBytes(type_string);
}