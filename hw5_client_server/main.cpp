#include <iostream>
#include "ServerSocket.h"
#include "MessageServer.h"
#include "MessageClient.h"
#include <future>

class MessageClientHandler {
 public:
  std::promise<std::string> comand_promise;
  std::string extra_string = "";
  volatile bool any_chat_requests = false;
  MessageClientHandler() = default;
  ~MessageClientHandler() = default;
  
  void print_header() {
    std::cout << "terminal_chat : " << extra_string << std::endl;
  }
  
  void on_message_receive(const messages::Message& msg) {
    std::cout << "                                                  <<<< "
              << msg.message_content() << std::endl;
    print_header();
  }
  
  void on_chat_disconnect() {
    std::cout << "chat is no longer exist" << std::endl;
    extra_string = "";
    print_header();
  }
  
  std::string on_chat_request(const std::vector<std::string>& partner_names) {
    any_chat_requests = true;
    comand_promise = std::promise<std::string>();
    std::cout << "chat requests : " << std::endl;
    for (const std::string& name : partner_names) {
      std::cout << name << std::endl;
    }
    std::cout << "choose partner : " << std::endl;
    std::string chosen_name = comand_promise.get_future().get();
    if (std::find(partner_names.begin(), partner_names.end(), chosen_name) !=
        partner_names.end()) {
      extra_string = "in chat with " + chosen_name;
      print_header();
    }
    return chosen_name;
  }
};

int main(int argc, char** args) {
  if (argc < 3) {
    std::cout
            << "please, run with 2 or 3 arguments : [\"server\" | \"client\"] , port, host?"
            << std::endl;
    return 0;
  }
  const auto mode = std::string(args[1]);
  int server_port = atoi(args[2]);
  const auto server_host =
          argc >= 4 ? std::string(args[3]) : InetUtils::kLoopbackAddress;
  if (mode == "server") {
    try {
      MessageServer server(InetSocketAddress(server_port, server_host));
      server.Run();
    } catch (...) {
    }
  } else {
    try {
      std::string comand;
      MessageClientHandler handler;
      MessageClient<MessageClientHandler> client(
              InetSocketAddress(server_port, server_host), &handler);
      while (true) {
        std::cout << "enter name : " << std::endl;
        std::string client_name;
        std::cin >> client_name;
        if (client.TryRegister(client_name))
          break;
        else
          std::cout << "failed to register with given name, please try again"
                    << std::endl;
      }
      while (true) {
        handler.print_header();
        std::cin >> comand;
        while (handler.any_chat_requests) {
          handler.any_chat_requests = false;
          handler.comand_promise.set_value(comand);
          std::cin >> comand;
        }
        if (comand == "users") {
          client.StopUpdates();
          std::cout << "Users List :" << std::endl;
          for (const std::string& user_name : client.GetUsers()) {
            std::cout << "  " << user_name << std::endl;
          }
          std::cout << "__________________" << std::endl;
          client.ContinueUpdates();
        } else if (comand == "start_chat") {
          if (client.state == MessageUtils::ClientState::IN_CHAT) {
            std::cout
                    << "already in chat, please, exit it before starting a new one"
                    << std::endl;
            continue;
          }
          client.StopUpdates();
          std::string name;
          std::cout << "enter user name to start chat with : ";
          std::cin >> name;
          std::cout << "waiting for " << name << "\'s responce" << std::endl;
          if (client.StartChatWith(name)) {
            handler.extra_string = "in chat with " + name;
          } else {
            std::cout << "failed to open chat with user " << name << std::endl;
          }
          client.ContinueUpdates();
        } else if (comand == "exit_chat") {
          if (client.state != MessageUtils::ClientState::IN_CHAT) {
            std::cout << "client not belong to any chat" << std::endl;
            continue;
          }
          client.StopUpdates();
          client.EndChat();
          client.ContinueUpdates();
          handler.extra_string = "";
        } else if (comand == "message") {
          if (client.state != MessageUtils::ClientState::IN_CHAT) {
            std::cout << "client not belong to any chat" << std::endl;
            continue;
          }
          std::string message;
          std::cout << ">>>>>>    ";
          std::cin >> message;
          client.StopUpdates();
          if (!client.SendChatMessage(message)) {
            std::cout << "partner exited the current chat" << std::endl;
            handler.extra_string = "";
          }
          client.ContinueUpdates();
        } else if (comand == "update_messages") {
          if (client.state != MessageUtils::ClientState::IN_CHAT) {
            std::cout << "client not belong to any chat" << std::endl;
            continue;
          }
          client.UpdateMessages();
          if (client.state == MessageUtils::ClientState::AVAILABLE) {
            std::cout << "partner disconnected from the chat" << std::endl;
            handler.extra_string = "";
          }
        } else if (comand == "disconnect") {
          client.Disconnect();
          break;
        } else {
          std::cout << "unknown comand \"" << comand << "\"" << std::endl;
        }
      }
      std::cout << "disconnected" << std::endl;
    } catch (...) {
    }
  }
  return 0;
}