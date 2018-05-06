cmake_minimum_required(VERSION 3.9)
project(chat)

set(CMAKE_CXX_STANDARD 17)

add_custom_command(
        OUTPUT message.pb.cc message.pb.h
        COMMAND protoc --cpp_out=./ message.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(
        OUTPUT session.pb.cc session.pb.h
        COMMAND protoc --cpp_out=./ session.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
        chat
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

target_link_libraries(chat protobuf)
