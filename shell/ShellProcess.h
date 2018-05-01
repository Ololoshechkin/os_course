//
// Created by Vadim on 21/04/2018.
//

#ifndef SHELL_SHELLPROCESS_H
#define SHELL_SHELLPROCESS_H

#include <string>

class ShellProcess {
 public:
  enum State {
    kWorking, kNormalExit, kErrorExit
  };
  ShellProcess();
  State ExecuteComand(const std::string& comand) const;
 private:
  static std::vector<std::string> SplitBySpace(const std::string& s);
  State ExecuteComand(std::vector<std::string>&& arguments) const;
  State ExecuteInCurrentProcess(std::vector<std::string>&& arguments) const;
  State WaitChildProcess(int pid) const;
  const static int kCallerProcessId;
  const static std::vector<std::string> kTerminalComand;
};

#endif //SHELL_SHELLPROCESS_H
