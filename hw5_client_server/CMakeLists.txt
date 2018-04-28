cmake_minimum_required(VERSION 3.9)
project(hw5_client_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        hw5_client_server
        main.cpp
        InetSocketAddress.cc InetSocketAddress.h
        ServerSocket.cc ServerSocket.h
        Socket.cc Socket.h
        SocketBase.cc SocketBase.h
        InetUtils.h InetUtils.cc
        MessageUnit.h MessageUnit.cc
        MessageServer.cc MessageServer.h
        MessageClient.cc MessageClient.h
        message.pb.cc message.pb.h
        session.pb.cc session.pb.h
)

target_link_libraries(hw5_client_server protobuf)
