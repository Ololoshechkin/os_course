#include <iostream>
#include "InetSocketAddress.h"
#include "KotlinNativeServer.h"
#include "AsyncClient.h"

int main(int argc, char** argv) {
  const auto args_message = [] {
    std::cout << "expected 2 or 3 arguments : mode : server/client, server port"
                 " and server host (optional)" << std::endl;
  };
  if (argc != 3 && argc != 4) {
    args_message();
    return 0;
  }
  if (strcmp(argv[1], "server") == 0) {
    try {
      InetSocketAddress address =
              argc == 3 ? InetSocketAddress(atoi(argv[2])) : InetSocketAddress(
                      atoi(argv[2]), argv[3]);
      KotlinNativeServer server(address);
      server.Run();
    } catch (const std::runtime_error& error) {
      std::cout << error.what() << std::endl;
    }
  } else if (strcmp(argv[1], "client") == 0) {
    try {
      InetSocketAddress address =
              argc == 3 ? InetSocketAddress(atoi(argv[2])) : InetSocketAddress(
                      atoi(argv[2]), argv[3]);
      AsyncClient client(address);
      client.Start();
    } catch (const std::runtime_error& error) {
      std::cout << error.what() << std::endl;
    }
  } else {
    args_message();
  }
  return 0;
}