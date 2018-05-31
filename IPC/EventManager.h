//
// Created by Vadim on 31/05/2018.
//

#ifndef IPC_EVENTMANAGER_H
#define IPC_EVENTMANAGER_H

#include <functional>
#include <map>
#include <sys/_types/_fd_def.h>
#include <sys/_types/_sigset_t.h>

class EventManager {
 private:
  fd_set read_fds;
  fd_set write_fds;
  fd_set error_fds;
  sigset_t mask;
  sigset_t signal_mask;
  int ndfs;
  std::map<int, std::function<void()>> fd_to_read_handler;
  std::map<int, std::function<void()>> fd_to_write_handler;
  std::map<int, std::function<void()>> fd_to_error_handler;
  std::map<int, std::function<void()>> signal_to_handler;
  
 public:
  EventManager();
  ~EventManager();
  void SubscribeRead(int fd, const std::function<void()>& handler);
  void SubscribeWrite(int fd, const std::function<void()>& handler);
  void SubscribeError(int fd, const std::function<void()>& handler);
  void SubscribeToSignal(int signal_id, const std::function<void()>& handler);
  void Start();
};

#endif //IPC_EVENTMANAGER_H
