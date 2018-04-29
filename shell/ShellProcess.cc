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
#include "ShellProcess.h"

const int ShellProcess::kCallerProcessId = 0;
const std::vector<std::string> ShellProcess::kTerminalComand = {"exit"};
ShellProcess::ShellProcess() = default;

ShellProcess::State ShellProcess::WaitChildProcess(int pid) const {
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("ERROR. Caught error in execution");
    };
    return kWorking;
}

ShellProcess::State
ShellProcess::ExecuteInCurrentProcess(std::vector<std::string>&& arguments) const {
    std::vector<char*> raw_chars;
    for (const auto& arg : arguments) {
        raw_chars.push_back(const_cast<char*>(arg.c_str()));
    }
    if (execvp(raw_chars[0], raw_chars.data()) < 0) {
        perror("unable to execute comand");
        return kErrorExit;
    }
    return kWorking;
}

ShellProcess::State
ShellProcess::ExecuteComand(std::vector<std::string>&& arguments) const {
    if (arguments == ShellProcess::kTerminalComand)
        return kNormalExit;
    auto pid = fork();
    if (pid < 0) {
        perror("failed to run new process");
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
