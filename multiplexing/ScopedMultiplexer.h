//
// Created by Vadim on 24/05/2018.
//

#ifndef MULTIPLEXING_SCOPEDEPOLL_H
#define MULTIPLEXING_SCOPEDEPOLL_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <utility>
#include <vector>
#include "InetSocketAddress.h"

class ScopedMultiplexer;

class Event {
 public:
  enum EventType {
    kInput, kOutput, kError, kDisconnect, kConnect
  };
  int file_descriptor;
  std::vector<EventType> event_types;
  Event();
  Event(
          int file_descriptor, std::vector<EventType> event_types
  );
  Event(const Event& other);
  void Swap(Event& other);
  bool operator==(const Event& other) const noexcept;
  Event& operator=(Event) noexcept;
  int GetEventMask() const;
 private:
  static int EventTypeToMask(EventType event_type) noexcept;
};

class ScopedMultiplexer {
 public:
  using Handler = std::function<bool (const Event&)>;
  static const int kMaxEventNumber = 1000;
  ScopedMultiplexer();
  ~ScopedMultiplexer();
  void SubscribeToEvent(const Event& event, const Handler& handler);
  void ChangeSubscription(const Event& event, const Handler& handler);
  void Unsubscribe(const Event& event);
  std::vector<Event> AwaitEvents();
  bool AwaitAndProcess(); // false if error/close/exit
 private:
  int mux_file_descriptor;
  std::unordered_map<int , Handler> fd_event_to_handler;
  void
  SubscribeToEventImpl(const Event& event, const Handler& handler, int mode);
};

#endif //MULTIPLEXING_SCOPEDEPOLL_H
