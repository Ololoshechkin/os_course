#include <iostream>
#include <libnet.h>
#include "KVDatabaseServer.h"
#include "KVClient.h"

const std::string kExit = "exit";

int main(int argc, char** argv) {
  if (strcmp(argv[1], "server") == 0) {
    try {
      KVDatabaseServer server(argv[2]);
      server.Run();
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
    }
  } else if (strcmp(argv[1], "client") == 0) {
    try {
      KVClient client(argv[2]);
      std::string comand;
      while (true) {
        std::cin >> comand;
        if (comand == kExit) {
          break;
        } else if (comand == KVClient::kSetValue) {
          std::string key, value;
          std::cin >> key >> value;
          client.Set(key, value);
        } else if (comand == KVClient::kGetValue) {
          std::string key;
          std::cin >> key;
          std::cout << client.Get(key) << std::endl;
        } else if (comand == KVClient::kGetKeys) {
          for (const std::string& key : client.GetKeys()) {
            std::cout << key << std::endl;
          }
        }
      }
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}