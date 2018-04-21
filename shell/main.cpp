#include <iostream>
#include <vector>
#include <sstream>
#include "ShellProcess.h"

int main(int argc, char** argv) {
    std::string command_with_args;
    auto draw_shell_header = [] { printf("╭─simple-shell\n╰─➤ $ "); };
    auto exit_shell = [](int code) { printf("goodbye!"); exit(code); };
    auto shell = ShellProcess();
    
    draw_shell_header();
    while (std::getline(std::cin, command_with_args)) {
        switch (shell.ExecuteComand(command_with_args)) {
            case ShellProcess::kNormalExit:
                exit_shell(0);
            case ShellProcess::kErrorExit:
                exit_shell(42);
            case ShellProcess::kWorking:
                draw_shell_header();
        }
    }
    return 0;
}