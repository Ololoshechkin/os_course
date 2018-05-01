#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <zconf.h>
#include "ShellProcess.h"

int main(int argc, char** argv) {
  std::string command_with_args;
  auto draw_shell_header = [] { printf("╭─simple-shell\n╰─➤ $ "); };
  auto shell = ShellProcess();
  draw_shell_header();
  while (std::getline(std::cin, command_with_args)) {
    switch (shell.ExecuteComand(command_with_args)) {
      case ShellProcess::kNormalExit:
        printf("goodbye!\n");
        printf("pid = %d\n", getpid());
        return 0;
      case ShellProcess::kErrorExit:
        printf("smth. went wrong... please, try again!\n");
        draw_shell_header();
        break;
      case ShellProcess::kWorking:
        draw_shell_header();
        break;
    }
  }
  return 0;
} 