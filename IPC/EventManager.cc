//
// Created by Vadim on 31/05/2018.
//

#include "EventManager.h"
#include "ExceptionHelp.h"
#include <algorithm>
#include <iostream>
#include <libnet.h>
#include <set>

static std::set<int> signal_set;

EventManager::EventManager() : ndfs(0) {
  sigemptyset(&signal_mask);
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  FD_ZERO(&error_fds);
}

void EventManager::SubscribeRead(int fd, const std::function<void()> &handler) {
  ndfs = std::max(ndfs, fd + 1);
  FD_SET(fd, &read_fds);
  fd_to_read_handler[fd] = handler;
}

void EventManager::SubscribeWrite(int fd,
                                  const std::function<void()> &handler) {
  ndfs = std::max(ndfs, fd + 1);
  FD_SET(fd, &write_fds);
  fd_to_write_handler[fd] = handler;
}

void EventManager::SubscribeError(int fd,
                                  const std::function<void()> &handler) {
  ndfs = std::max(ndfs, fd + 1);
  FD_SET(fd, &error_fds);
  fd_to_error_handler[fd] = handler;
}

void EventManager::SubscribeToSignal(int signal_id,
                                     const std::function<void()> &handler) {
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = [](int sig) { signal_set.insert(sig); };
  if (sigaction(signal_id, &action, nullptr) < 0) {
    throw std::runtime_error(GetErrorMessage("failed to invoke sigaction"));
  }
  sigaddset(&mask, signal_id);
  signal_to_handler[signal_id] = handler;
}

void EventManager::HandleSignals() {
  for (int signal : signal_set) {
    signal_to_handler[signal]();
  }
  signal_set.clear();
  errno = 0;
}

void EventManager::Start() {
  while (true) {
    HandleSignals();
    int res =
        pselect(ndfs, &read_fds, &write_fds, &error_fds, nullptr, &signal_mask);
    if (res < 0) {
      if (errno == EINTR) {
        HandleSignals();
      } else {
        break;
      }
    } else {
      for (const auto &it : fd_to_read_handler) {
        if (FD_ISSET(it.first, &read_fds)) {
          it.second();
        }
      }
      for (const auto &it : fd_to_write_handler) {
        if (FD_ISSET(it.first, &write_fds)) {
          it.second();
        }
      }
      for (const auto &it : fd_to_error_handler) {
        if (FD_ISSET(it.first, &error_fds)) {
          it.second();
        }
      }
    }
  }
}

EventManager::~EventManager() = default;
