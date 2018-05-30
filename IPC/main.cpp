#include <iostream>
#include <libnet.h>
#include "KVDatabaseServer.h"
#include "KVClient.h"

const std::string kExit = "exit";

int main(int argc, char** argv) {
  if (strcmp(argv[1], "server") == 0) {
    try {
      KVDatabaseServer s(argv[2]);
      s.Run();
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
        }
      }
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}