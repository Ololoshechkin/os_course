//
// Created by Vadim on 26/05/2018.
//

#include <iostream>
#include "KotlinNativeServer.h"

const char KotlinNativeServer::kQueryEnd = '#';

KotlinNativeServer::KotlinNativeServer(const InetSocketAddress& address) :
        server_socket(address) {
  server_socket.SetBlocking(false);
};
KotlinNativeServer::~KotlinNativeServer() = default;

void KotlinNativeServer::Run() {
  const auto on_accept_handler = [this](
          const Event& accept_event
  ) -> bool {
    const auto client = server_socket.accept();
    std::string received_buffer;
    const auto client_events_handler = GetClientEventHandler(
            client, received_buffer);
    multiplexer.SubscribeToEvent(
            client->GetReceiveEvent(), client_events_handler);
    return true;
  };
  std::cout << "multiplexer.SubscribeToEvent(\n"
               "          server_socket.GetAcceptEvent(), on_accept_handler);"
            << std::endl;
  multiplexer.SubscribeToEvent(
          server_socket.GetAcceptEvent(), on_accept_handler);
  std::cout << "OK" << std::endl;
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
        std::shared_ptr<Socket> client, std::string received_bytes
) {
  const ScopedMultiplexer::Handler client_events_handler = [this,
                                                            &received_bytes,
                                                            &client_events_handler,
                                                            client
  ](const Event& event) -> bool {
    std::for_each(
            event.event_types.begin(), event.event_types.end(),
            [this, &client_events_handler, &received_bytes, &event, client](
                    Event::EventType type
            ) {
              switch (type) {
                case Event::kInput: {
                  received_bytes += client->ReadBytes();
                  CheckAndChangeSubscription(
                          received_bytes, client, client_events_handler);
                  break;
                }
                case Event::kOutput: {
                  /* while there is kQueryEnd in <tt>received_bytes</tt> 
                   * and while we don't block on sending reply
                  */
                  while (true) {
                    size_t request_end = std::find(
                            received_bytes.begin(), received_bytes.end(),
                            kQueryEnd) - received_bytes.begin();
                    std::string reply = ProcessRequest(
                            received_bytes.substr(0, request_end));
                    if (client->TryWriteBytes(reply)) {
                      received_bytes = received_bytes.substr(
                              request_end + 1,
                              received_bytes.size() - request_end - 1);
                    } else {
                      break;
                    }
                  }
                  CheckAndChangeSubscription(
                          received_bytes, client, client_events_handler);
                  break;
                }
                default: {
                  multiplexer.Unsubscribe(event);
                  break;
                }
              }
            });
    return true;
  };
  return client_events_handler;
}
