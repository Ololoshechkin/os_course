//
// Created by Vadim on 26/05/2018.
//

#include <zconf.h>
#include <iostream>
#include "AsyncClient.h"
#include "KotlinNativeServer.h"

const std::string AsyncClient::kExitMessage = "exit"; // NOLINT

AsyncClient::AsyncClient(const InetSocketAddress& address) :
        socket(address), requests_in_porcess(0) {
  socket.SetBlocking(false);
}

void AsyncClient::Start() {
  const ScopedMultiplexer::Handler socket_event_handler = [this,
                                                           &socket_event_handler
  ](
          const Event& event
  ) -> bool {
    return std::all_of(
            event.event_types.begin(), event.event_types.end(),
            [this, &event, &socket_event_handler](
                    Event::EventType type
            ) -> bool {
              bool should_continue = true;
              switch (type) {
                case Event::EventType::kInput: {
                  current_received_buffer += socket.ReadBytes();
                  ProcessBuffer();
                  CheckAndUpdateSubscriptions(
                          event, socket_event_handler);
                  break;
                }
                case Event::EventType::kOutput: {
                  std::string request = input_strings.back();
                  if (socket.TryWriteBytes(request)) {
                    ++requests_in_porcess;
                  }
                  CheckAndUpdateSubscriptions(
                          event, socket_event_handler);
                  break;
                }
                default: {
                  std::cout << "disconnecting..." << std::endl;
                  should_continue = false;
                  break;
                }
              }
              return should_continue;
            });
  };
  multiplexer.SubscribeToEvent(
          Event(STDIN_FILENO, {Event::EventType::kInput}),
          [this, &socket_event_handler](const Event& e) -> bool {
            std::string message;
            getline(std::cin, message);
            if (message == kExitMessage) {
              return false;
            } else {
              input_strings.push_back(message);
              CheckAndUpdateSubscriptions(
                      socket.GetSendEvent(), socket_event_handler);
              return true;
            }
          });
  while (multiplexer.AwaitAndProcess());
}

void AsyncClient::ProcessBuffer() {
  // while there is kQueryEnd in buffer
  while (true) {
    const auto pos = std::find(
            current_received_buffer.begin(), current_received_buffer.end(),
            KotlinNativeServer::kQueryEnd);
    if (pos == current_received_buffer.end()) {
      break;
    } else {
      const auto end_pos = pos - current_received_buffer.begin();
      std::cout << "[server] : " << current_received_buffer.substr(0, end_pos)
                << std::endl;
      current_received_buffer = current_received_buffer.substr(
              end_pos + 1, current_received_buffer.size() - end_pos - 1);
      --requests_in_porcess;
    }
  }
}

void AsyncClient::CheckAndUpdateSubscriptions(
        const Event& event, const ScopedMultiplexer::Handler& events_handler
) {
  if (input_strings.empty() && requests_in_porcess == 0) {
    multiplexer.Unsubscribe(event);
    socket_is_subscribed = false;
  } else {
    if (input_strings.empty()) {
      if (socket_is_subscribed)
        multiplexer.ChangeSubscription(socket.GetSendEvent(), events_handler);
      else
        multiplexer.SubscribeToEvent(socket.GetSendEvent(), events_handler);
    } else if (requests_in_porcess == 0) {
      if (socket_is_subscribed)
        multiplexer.ChangeSubscription(
                socket.GetReceiveEvent(), events_handler);
      else
        multiplexer.SubscribeToEvent(socket.GetReceiveEvent(), events_handler);
    } else {
      if (socket_is_subscribed)
        multiplexer.ChangeSubscription(
                socket.GetSendAndReceiveEvent(), events_handler);
      else
        multiplexer.SubscribeToEvent(
                socket.GetSendAndReceiveEvent(), events_handler);
    }
    socket_is_subscribed = true;
  }
}

AsyncClient::~AsyncClient() = default;
