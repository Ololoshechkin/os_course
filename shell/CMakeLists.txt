cmake_minimum_required(VERSION 2.6)
project(shell)

set(CMAKE_CXX_STANDARD 11)

add_executable(shell main.cpp ShellProcess.cc ShellProcess.h)