//
// Created by Vadim on 24/05/2018.
//

#include <algorithm>
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <numeric>
#include <unistd.h>
#include "ScopedMultiplexer.h"
#include "ExceptionHelp.h"

#define UNIX 0
#define MAC_OS  1
#if defined(__unix__)
#define OS UNIX
#elif defined(__APPLE__)
#define OS MAC_OS
#endif
#if OS == OS_UNIX
                                                                                                                        
#include <sys/epoll.h>
#include <sys/types.h>

#elif OS == MAC_OS

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/errno.h>

#endif

ScopedMultiplexer::~ScopedMultiplexer() {
  if (mux_file_descriptor != -1 && close(mux_file_descriptor) == -1) {
    std::cout << GetErrorMessage("failed to close multiplexor") << std::endl;
  }
}

bool Event::operator==(const Event& other) const noexcept {
  return file_descriptor == other.file_descriptor &&
         event_types == other.event_types;
}

int Event::GetEventMask() const {
  return std::accumulate(
          event_types.begin(), event_types.end(), 0,
          [](int a, EventType b) -> int { return a | EventTypeToMask(b); });
}

bool ScopedMultiplexer::AwaitAndProcess() {
  const auto events = AwaitEvents();
  return std::all_of(
          events.begin(), events.end(), [this](const Event& event) -> bool {
            if (fd_event_to_handler.count(event.file_descriptor)) {
              return fd_event_to_handler[event.file_descriptor](
                      event);
            } else {
              return true;
            }
          });
}

Event::Event(const Event& other) :
        file_descriptor(other.file_descriptor),
        event_types(other.event_types) {}

void Event::Swap(Event& other) {
  int tmp = other.file_descriptor;
  other.file_descriptor = file_descriptor;
  file_descriptor = tmp;
  std::swap(event_types, other.event_types);
}

Event& Event::operator=(Event other) noexcept {
  Swap(other);
  return *this;
}

Event::Event(
        int file_descriptor, const std::vector<Event::EventType>& event_types,
        const std::function<void()>& deleter
) :
        file_descriptor(file_descriptor), event_types(event_types),
        deleter(deleter) {}

Event::Event() :
        Event(0, {}) {
}

Event::~Event() {
  deleter();
}

#if OS == UNIX
                                                                                                                        
ScopedMultiplexer::ScopedMultiplexer() :
        mux_file_descriptor(epoll_create(1)) {
  if (mux_file_descriptor == -1) {
    throw std::runtime_error(GetErrorMessage("Error creating epoll"));
  }
}

void ScopedMultiplexer::SubscribeToEventImpl(
        const Event& event, const ScopedMultiplexer::Handler& handler, int mode
) {
  struct epoll_event system_event{};
  system_event.events = event.GetEventMask();
  system_event.data.fd = event.file_descriptor;
  if (epoll_ctl(mux_file_descriptor, mode,event.file_descriptor,
                      &system_event) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to subscribe to the new event"));
  }
  if (!fd_event_to_handler.count(event.file_descriptor))
    fd_event_to_handler[event.file_descriptor] = handler;
}

void ScopedMultiplexer::SubscribeToEvent(
        const Event& event, const ScopedMultiplexer::Handler& handler
) {
  SubscribeToEventImpl(event, handler, EPOLL_CTL_ADD);
}


void ScopedMultiplexer::ChangeSubscription(
        const Event& event, const Handler& handler
) {
    SubscribeToEventImpl(event, handler, EPOLL_CTL_MOD);
}

int Event::EventTypeToMask(Event::EventType event_type) noexcept {
  switch (event_type) {
    case Event::kInput:
      return EPOLLIN;
    case Event::kOutput:
      return EPOLLOUT;
    case Event::kError:
      return EPOLLERR;
    case Event::kDisconnect:
      return EPOLLRDHUP;
    default:
      return 0;
  }
}


void ScopedMultiplexer::Unsubscribe(const Event& event) {
  fd_event_to_handler.erase(event.file_descriptor);
  if (epoll_ctl(mux_file_descriptor, EPOLL_CTL_DEL, event.file_descriptor, nullptr) ==
      -1) {
    throw std::runtime_error(
            GetErrorMessage("failed to unsubscribe from event"));
  }
}

Event FromEpollEevent(const struct epoll_event& e_event) {
  const auto mask = e_event.events;
  std::vector<Event::EventType> types{};
  if (mask & EPOLLIN)
    types.push_back(Event::EventType::kInput);
  if (mask & EPOLLOUT)
    types.push_back(Event::EventType::kOutput);
  if (mask & EPOLLERR)
    types.push_back(Event::EventType::kError);
  if (mask & EPOLLRDHUP || mask & EPOLLHUP)
    types.push_back(Event::EventType::kDisconnect);
  return Event(static_cast<int>(e_event.data.fd), types);
}

std::vector<Event> ScopedMultiplexer::AwaitEvents() {
  epoll_event events_array[kMaxEventNumber];
  int event_count;
  if ((event_count = epoll_wait(
          mux_file_descriptor, events_array, kMaxEventNumber, -1)) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to wait for events"));
  }
  std::vector<Event> result_events(event_count);
  std::transform(
          events_array, events_array + event_count, result_events.begin(),
          [&result_events](const struct epoll_event& e_event) -> Event {
            return FromEpollEevent(e_event);
          });
  return result_events;
}


#elif OS == MAC_OS
static struct kevent ev_set;

ScopedMultiplexer::ScopedMultiplexer() :
        mux_file_descriptor(kqueue()) {
  if (mux_file_descriptor == -1) {
    throw std::runtime_error(GetErrorMessage("Error creating epoll"));
  }
}

void ScopedMultiplexer::SubscribeToEventImpl(
        const Event& event, const ScopedMultiplexer::Handler& handler, int mode
) {
  EV_SET(&ev_set, event.file_descriptor, event.GetEventMask(), EV_ADD, 0, 0,
         nullptr);
  if (kevent(mux_file_descriptor, &ev_set, 1, nullptr, 0, nullptr) < 0) {
    throw std::runtime_error(
            GetErrorMessage("failed to subscribe to the new event"));
  }
  fd_event_to_handler[event.file_descriptor] = handler;
}

void ScopedMultiplexer::SubscribeToEvent(
        const Event& event, const ScopedMultiplexer::Handler& handler
) {
  SubscribeToEventImpl(event, handler, 0);
}

void ScopedMultiplexer::ChangeSubscription(
        const Event& event, const Handler& handler
) {
  // mac processes re-subscribe case in an appropriate way
  SubscribeToEvent(event, handler);
}

int Event::EventTypeToMask(Event::EventType event_type) noexcept {
  switch (event_type) {
    case Event::kInput:
      return EVFILT_READ;
    case Event::kOutput:
      return EVFILT_WRITE;
    case Event::kError:
      return EVFILT_EXCEPT;
    case Event::kDisconnect:
      return EV_EOF;
    default:
      return 0;
  }
}

void ScopedMultiplexer::Unsubscribe(const Event& event) {
  if (fd_event_to_handler.count(event.file_descriptor)) {
    fd_event_to_handler.erase(event.file_descriptor);
    EV_SET(&ev_set, event.file_descriptor, event.GetEventMask(), EV_DELETE, 0,
           0, nullptr);
    if (kevent(mux_file_descriptor, &ev_set, 1, nullptr, 0, nullptr) == -1) {
      throw std::runtime_error(
              GetErrorMessage("failed to unsubscribe from event"));
    }
  }
}

Event FromKevent(const struct kevent k_event) {
  uint32_t mask = k_event.fflags;
  std::vector<Event::EventType> types{};
  if (mask & EVFILT_READ)
    types.push_back(Event::EventType::kInput);
  if (mask & EVFILT_WRITE)
    types.push_back(Event::EventType::kOutput);
  if (mask & EVFILT_EXCEPT)
    types.push_back(Event::EventType::kError);
  if (mask & EV_EOF)
    types.push_back(Event::EventType::kDisconnect);
  return Event(static_cast<int>(k_event.ident), types);
}

std::vector<Event> ScopedMultiplexer::AwaitEvents() {
  static struct kevent events_array[kMaxEventNumber];
  int event_count = kevent(
          mux_file_descriptor, nullptr, 0, events_array, kMaxEventNumber,
          nullptr);
  if (event_count < 1) {
    throw std::runtime_error(GetErrorMessage("failed to wait for events"));
  }
  std::vector<Event> result_events(event_count);
  std::transform(
          events_array, events_array + event_count, result_events.begin(),
          [&result_events](const struct kevent k_event) -> Event {
            return FromKevent(k_event);
          });
  return result_events;
}

#endif
