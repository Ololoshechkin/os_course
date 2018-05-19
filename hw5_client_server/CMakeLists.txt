cmake_minimum_required(VERSION 3.5)
project(chat)

set(CMAKE_CXX_STANDARD 14)

message("CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR : ${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
        OUTPUT message.pb.cc message.pb.h
        COMMAND protoc --cpp_out=${CMAKE_CURRENT_BINARY_DIR} message.proto
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(
        OUTPUT session.pb.cc session.pb.h
        COMMAND protoc --cpp_out=${CMAKE_CURRENT_BINARY_DIR} session.proto
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(
        chat
        main.cpp
        InetSocketAddress.cc InetSocketAddress.h
        ServerSocket.cc ServerSocket.h
        Socket.cc Socket.h
        SocketBase.cc SocketBase.h
        InetUtils.h InetUtils.cc
        MessageUtils.h MessageUtils.cc
        MessageServer.cc MessageServer.h
        MessageClient.h
        message.pb.cc message.pb.h
        session.pb.cc session.pb.h
)

target_link_libraries(chat protobuf pthread)
