//
// Created by Vadim on 21/04/2018.
//

#include <vector>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <zconf.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "ShellProcess.h"

const int ShellProcess::kCallerProcessId = 0;
const std::vector<std::string> ShellProcess::kTerminalComand = {"exit"};
ShellProcess::ShellProcess() = default;

ShellProcess::State ShellProcess::WaitChildProcess(int pid) const {
  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("failed to wait for child process termination");
    errno = 0;
  }
  return kWorking;
}

ShellProcess::State ShellProcess::ExecuteInCurrentProcess(
        std::vector<
                std::string
        >&& arguments
) const {
  char** argv = new char* [arguments.size()];
  for (size_t i = 0; i < arguments.size(); ++i) {
    argv[i] = const_cast<char*>(arguments[i].c_str());
  }
  std::cout << "                   argv address : " << argv << std::endl;
  if (execvp(argv[0], argv) < 0) {
    perror("unable to execute comand");
    errno = 0;
    delete [] argv;
    exit(42);
  }
  delete [] argv;
  return kWorking;
}

ShellProcess::State
ShellProcess::ExecuteComand(std::vector<std::string>&& arguments) const {
  if (arguments == ShellProcess::kTerminalComand)
    return kNormalExit;
  if (arguments.empty())
    return kWorking;
  auto pid = fork();
  if (pid < 0) {
    perror("failed to run new process");
    errno = 0;
    return kWorking;
  }
  if (pid == kCallerProcessId) {
    return ExecuteInCurrentProcess(std::move(arguments));
  } else {
    return WaitChildProcess(pid);
  }
}

std::vector<std::string> ShellProcess::SplitBySpace(const std::string& s) {
  auto word_stream = std::stringstream(s);
  std::string item;
  std::vector<std::string> elems;
  while (word_stream >> item) {
    elems.emplace_back(std::move(item));
  }
  return elems;
}

ShellProcess::State
ShellProcess::ExecuteComand(const std::string& comand) const {
  return ExecuteComand(SplitBySpace(comand));
}
