//
// Created by Vadim on 26/05/2018.
//

#include <algorithm>
#include <iostream>
#include "KotlinNativeServer.h"

const char KotlinNativeServer::kQueryEnd = '#';

KotlinNativeServer::KotlinNativeServer(const InetSocketAddress& address) :
        server_socket(address) {
};
KotlinNativeServer::~KotlinNativeServer() = default;

void KotlinNativeServer::Run() {
  const auto on_accept_handler = [this](
          const Event& accept_event
  ) -> bool {
    const auto client = server_socket.accept();
    std::string received_buffer;
    const auto client_events_handler = GetClientEventHandler(
            client);
    multiplexer.SubscribeToEvent(
            client->GetReceiveEvent(), client_events_handler);
    return true;
  };
  multiplexer.SubscribeToEvent(
          server_socket.GetAcceptEvent(), on_accept_handler);
  while (multiplexer.AwaitAndProcess());
}

std::string KotlinNativeServer::ProcessRequest(std::string&& request) {
  return "You said \"" + request + "\", and I say : Kotlin Native!!!" +
         kQueryEnd;
}

void KotlinNativeServer::CheckAndChangeSubscription(
        const std::string& received_bytes, std::shared_ptr<Socket> client,
        const Handler& client_events_handler
) {
  if (std::count(received_bytes.begin(), received_bytes.end(), kQueryEnd)) {
    multiplexer.ChangeSubscription(
            client->GetSendAndReceiveEvent(), client_events_handler);
  } else {
    multiplexer.ChangeSubscription(
            client->GetReceiveEvent(), client_events_handler);
  }
}

KotlinNativeServer::Handler KotlinNativeServer::GetClientEventHandler(
        std::shared_ptr<Socket> client
) {
  const ScopedMultiplexer::Handler client_events_handler = [this,
                                                            &client_events_handler,
                                                            client
  ](const Event& event) -> bool {
    std::cout << "event occured on fd = " << event.file_descriptor << std::endl;
    std::for_each(
            event.event_types.begin(), event.event_types.end(),
            [this, &client_events_handler, &event, client](
                    Event::EventType type
            ) {
              switch (type) {
                case Event::kInput: {
                  std::cout << "Event::kInput" << std::endl;
                  fd_to_receive_buffer[event.file_descriptor] += client->ReadBytes();
                  std::cout << "received_bytes : " << fd_to_receive_buffer[event.file_descriptor] << std::endl;
                  CheckAndChangeSubscription(
                          fd_to_receive_buffer[event.file_descriptor], client, client_events_handler);
                  break;
                }
                case Event::kOutput: {
                  std::cout << "Event::kOutput" << std::endl;
                  /* while there is kQueryEnd in <tt>received_bytes</tt> 
                   * and while we don't block on sending reply
                  */
                  while (true) {
                    auto received_bytes = fd_to_receive_buffer[event.file_descriptor];
                    size_t request_end = std::find(
                            received_bytes.begin(), received_bytes.end(),
                            kQueryEnd) - received_bytes.begin();
                    if (request_end ==
                        received_bytes.end() - received_bytes.begin()) {
                      break;
                    }
                    if (fd_to_send_buffer[event.file_descriptor].empty()) {
                      fd_to_send_buffer[event.file_descriptor] = ProcessRequest(
                              received_bytes.substr(0, request_end));
                    }
                    client->TryWriteBytes(
                            fd_to_send_buffer[event.file_descriptor]);
                    if (fd_to_send_buffer[event.file_descriptor].empty()) {
                      fd_to_receive_buffer[event.file_descriptor] = received_bytes.substr(
                              request_end + 1,
                              received_bytes.size() - request_end - 1);
                    } else {
                      break;
                    }
                  }
                  CheckAndChangeSubscription(
                          fd_to_receive_buffer[event.file_descriptor], client,
                          client_events_handler);
                  break;
                }
                default: {
                  std::cout << "Event::kError||kDisconnect" << std::endl;
                  multiplexer.Unsubscribe(event);
                  break;
                }
              }
            });
    return true;
  };
  return client_events_handler;
}
