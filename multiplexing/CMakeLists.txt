cmake_minimum_required(VERSION 3.10)
project(multiplexing)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        multiplexing

        main.cpp
        SocketScopedBase.cc
        Socket.cc
        ServerSocket.cc
        InetSocketAddress.cc
        ScopedMultiplexer.cc
        ExceptionHelp.cc ExceptionHelp.h KotlinNativeServer.cc KotlinNativeServer.h AsyncClient.cc AsyncClient.h)