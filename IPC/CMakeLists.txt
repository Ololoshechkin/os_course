cmake_minimum_required(VERSION 3.5)
project(IPC)

set(CMAKE_CXX_STANDARD 14)

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(
        IPC

        main.cpp
        KVDatabaseServer.cc
        ScopedUnixSocket.cc
        ZygoteDatabaseService.cc
        ExceptionHelp.cc
        KVClient.cc KVClient.h ScopedPipe.cc ScopedPipe.h EventManager.cc EventManager.h)
