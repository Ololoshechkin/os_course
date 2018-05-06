cmake_minimum_required(VERSION 3.10)
project(jit)

set(CMAKE_CXX_STANDARD 17)

add_executable(jit main.cpp Memory.cc Memory.h CodeWithCycles.cc CodeWithCycles.h)