cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        IPC

        main.cpp
        KVDatabaseServer.cc
        ScopedUnixSocket.cc
        ZygoteDatabaseService.cc
        ExceptionHelp.cc
        KVClient.cc KVClient.h)