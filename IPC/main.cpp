#include <iostream>
#include "KVDatabaseServer.h"
#include "KVClient.h"

const std::string kExit = "exit";

int main(int argc, char** argv) {
  const auto args_info = [] {
    std::cout << "please, run with 2 arguments : \"client\" / \"server\""
                 " and filename of the Server's Unix Domain socket"
              << std::endl;
  };
  if (argc != 3) {
    args_info();
    return 0;
  }
  if (strcmp(argv[1], "server") == 0) {
    try {
      KVDatabaseServer server(argv[2]);
      server.Run();
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
    }
  } else if (strcmp(argv[1], "client") == 0) {
    std::cout << "Client Mode:" << std::endl;
    std::cout << "Usage : [keyset | set KEY VALUE | get KEY | exit]" << std::endl;
    std::cout << "  keyset - list of keys associated with any value" << std::endl;
    std::cout << "  set KEY VALUE - store VALUE associated with KEY" << std::endl;
    std::cout << "  get KEY - get the VALUE associated with the KEY" << std::endl;
    std::cout << "  exit - exit the application" << std::endl << std::endl;
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
  } else {
    args_info();
  }
  return 0;
}