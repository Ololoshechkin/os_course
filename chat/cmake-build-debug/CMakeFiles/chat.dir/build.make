# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Vadim/Documents/OS_course/hw5_client_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/chat.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chat.dir/flags.make

message.pb.cc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating message.pb.cc, message.pb.h"
	cd /Users/Vadim/Documents/OS_course/hw5_client_server && protoc --cpp_out=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug message.proto

message.pb.h: message.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate message.pb.h

session.pb.cc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating session.pb.cc, session.pb.h"
	cd /Users/Vadim/Documents/OS_course/hw5_client_server && protoc --cpp_out=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug session.proto

session.pb.h: session.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate session.pb.h

CMakeFiles/chat.dir/main.cpp.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/chat.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/main.cpp.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/main.cpp

CMakeFiles/chat.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/main.cpp > CMakeFiles/chat.dir/main.cpp.i

CMakeFiles/chat.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/main.cpp -o CMakeFiles/chat.dir/main.cpp.s

CMakeFiles/chat.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/chat.dir/main.cpp.o.requires

CMakeFiles/chat.dir/main.cpp.o.provides: CMakeFiles/chat.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/chat.dir/main.cpp.o.provides

CMakeFiles/chat.dir/main.cpp.o.provides.build: CMakeFiles/chat.dir/main.cpp.o


CMakeFiles/chat.dir/InetSocketAddress.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/InetSocketAddress.cc.o: ../InetSocketAddress.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/chat.dir/InetSocketAddress.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/InetSocketAddress.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/InetSocketAddress.cc

CMakeFiles/chat.dir/InetSocketAddress.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/InetSocketAddress.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/InetSocketAddress.cc > CMakeFiles/chat.dir/InetSocketAddress.cc.i

CMakeFiles/chat.dir/InetSocketAddress.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/InetSocketAddress.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/InetSocketAddress.cc -o CMakeFiles/chat.dir/InetSocketAddress.cc.s

CMakeFiles/chat.dir/InetSocketAddress.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/InetSocketAddress.cc.o.requires

CMakeFiles/chat.dir/InetSocketAddress.cc.o.provides: CMakeFiles/chat.dir/InetSocketAddress.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/InetSocketAddress.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/InetSocketAddress.cc.o.provides

CMakeFiles/chat.dir/InetSocketAddress.cc.o.provides.build: CMakeFiles/chat.dir/InetSocketAddress.cc.o


CMakeFiles/chat.dir/ServerSocket.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/ServerSocket.cc.o: ../ServerSocket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/chat.dir/ServerSocket.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/ServerSocket.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/ServerSocket.cc

CMakeFiles/chat.dir/ServerSocket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/ServerSocket.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/ServerSocket.cc > CMakeFiles/chat.dir/ServerSocket.cc.i

CMakeFiles/chat.dir/ServerSocket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/ServerSocket.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/ServerSocket.cc -o CMakeFiles/chat.dir/ServerSocket.cc.s

CMakeFiles/chat.dir/ServerSocket.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/ServerSocket.cc.o.requires

CMakeFiles/chat.dir/ServerSocket.cc.o.provides: CMakeFiles/chat.dir/ServerSocket.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/ServerSocket.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/ServerSocket.cc.o.provides

CMakeFiles/chat.dir/ServerSocket.cc.o.provides.build: CMakeFiles/chat.dir/ServerSocket.cc.o


CMakeFiles/chat.dir/Socket.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/Socket.cc.o: ../Socket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/chat.dir/Socket.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/Socket.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/Socket.cc

CMakeFiles/chat.dir/Socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/Socket.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/Socket.cc > CMakeFiles/chat.dir/Socket.cc.i

CMakeFiles/chat.dir/Socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/Socket.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/Socket.cc -o CMakeFiles/chat.dir/Socket.cc.s

CMakeFiles/chat.dir/Socket.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/Socket.cc.o.requires

CMakeFiles/chat.dir/Socket.cc.o.provides: CMakeFiles/chat.dir/Socket.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/Socket.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/Socket.cc.o.provides

CMakeFiles/chat.dir/Socket.cc.o.provides.build: CMakeFiles/chat.dir/Socket.cc.o


CMakeFiles/chat.dir/SocketScopedBase.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/SocketScopedBase.cc.o: ../SocketScopedBase.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/chat.dir/SocketScopedBase.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/SocketScopedBase.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/SocketScopedBase.cc

CMakeFiles/chat.dir/SocketScopedBase.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/SocketScopedBase.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/SocketScopedBase.cc > CMakeFiles/chat.dir/SocketScopedBase.cc.i

CMakeFiles/chat.dir/SocketScopedBase.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/SocketScopedBase.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/SocketScopedBase.cc -o CMakeFiles/chat.dir/SocketScopedBase.cc.s

CMakeFiles/chat.dir/SocketScopedBase.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/SocketScopedBase.cc.o.requires

CMakeFiles/chat.dir/SocketScopedBase.cc.o.provides: CMakeFiles/chat.dir/SocketScopedBase.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/SocketScopedBase.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/SocketScopedBase.cc.o.provides

CMakeFiles/chat.dir/SocketScopedBase.cc.o.provides.build: CMakeFiles/chat.dir/SocketScopedBase.cc.o


CMakeFiles/chat.dir/InetUtils.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/InetUtils.cc.o: ../InetUtils.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/chat.dir/InetUtils.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/InetUtils.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/InetUtils.cc

CMakeFiles/chat.dir/InetUtils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/InetUtils.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/InetUtils.cc > CMakeFiles/chat.dir/InetUtils.cc.i

CMakeFiles/chat.dir/InetUtils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/InetUtils.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/InetUtils.cc -o CMakeFiles/chat.dir/InetUtils.cc.s

CMakeFiles/chat.dir/InetUtils.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/InetUtils.cc.o.requires

CMakeFiles/chat.dir/InetUtils.cc.o.provides: CMakeFiles/chat.dir/InetUtils.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/InetUtils.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/InetUtils.cc.o.provides

CMakeFiles/chat.dir/InetUtils.cc.o.provides.build: CMakeFiles/chat.dir/InetUtils.cc.o


CMakeFiles/chat.dir/MessageUtils.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/MessageUtils.cc.o: ../MessageUtils.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/chat.dir/MessageUtils.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/MessageUtils.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/MessageUtils.cc

CMakeFiles/chat.dir/MessageUtils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/MessageUtils.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/MessageUtils.cc > CMakeFiles/chat.dir/MessageUtils.cc.i

CMakeFiles/chat.dir/MessageUtils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/MessageUtils.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/MessageUtils.cc -o CMakeFiles/chat.dir/MessageUtils.cc.s

CMakeFiles/chat.dir/MessageUtils.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/MessageUtils.cc.o.requires

CMakeFiles/chat.dir/MessageUtils.cc.o.provides: CMakeFiles/chat.dir/MessageUtils.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/MessageUtils.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/MessageUtils.cc.o.provides

CMakeFiles/chat.dir/MessageUtils.cc.o.provides.build: CMakeFiles/chat.dir/MessageUtils.cc.o


CMakeFiles/chat.dir/MessageServer.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/MessageServer.cc.o: ../MessageServer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/chat.dir/MessageServer.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/MessageServer.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/MessageServer.cc

CMakeFiles/chat.dir/MessageServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/MessageServer.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/MessageServer.cc > CMakeFiles/chat.dir/MessageServer.cc.i

CMakeFiles/chat.dir/MessageServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/MessageServer.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/MessageServer.cc -o CMakeFiles/chat.dir/MessageServer.cc.s

CMakeFiles/chat.dir/MessageServer.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/MessageServer.cc.o.requires

CMakeFiles/chat.dir/MessageServer.cc.o.provides: CMakeFiles/chat.dir/MessageServer.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/MessageServer.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/MessageServer.cc.o.provides

CMakeFiles/chat.dir/MessageServer.cc.o.provides.build: CMakeFiles/chat.dir/MessageServer.cc.o


CMakeFiles/chat.dir/message.pb.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/message.pb.cc.o: message.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/chat.dir/message.pb.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/message.pb.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/message.pb.cc

CMakeFiles/chat.dir/message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/message.pb.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/message.pb.cc > CMakeFiles/chat.dir/message.pb.cc.i

CMakeFiles/chat.dir/message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/message.pb.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/message.pb.cc -o CMakeFiles/chat.dir/message.pb.cc.s

CMakeFiles/chat.dir/message.pb.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/message.pb.cc.o.requires

CMakeFiles/chat.dir/message.pb.cc.o.provides: CMakeFiles/chat.dir/message.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/message.pb.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/message.pb.cc.o.provides

CMakeFiles/chat.dir/message.pb.cc.o.provides.build: CMakeFiles/chat.dir/message.pb.cc.o


CMakeFiles/chat.dir/session.pb.cc.o: CMakeFiles/chat.dir/flags.make
CMakeFiles/chat.dir/session.pb.cc.o: session.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/chat.dir/session.pb.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chat.dir/session.pb.cc.o -c /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/session.pb.cc

CMakeFiles/chat.dir/session.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chat.dir/session.pb.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/session.pb.cc > CMakeFiles/chat.dir/session.pb.cc.i

CMakeFiles/chat.dir/session.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chat.dir/session.pb.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/session.pb.cc -o CMakeFiles/chat.dir/session.pb.cc.s

CMakeFiles/chat.dir/session.pb.cc.o.requires:

.PHONY : CMakeFiles/chat.dir/session.pb.cc.o.requires

CMakeFiles/chat.dir/session.pb.cc.o.provides: CMakeFiles/chat.dir/session.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/chat.dir/build.make CMakeFiles/chat.dir/session.pb.cc.o.provides.build
.PHONY : CMakeFiles/chat.dir/session.pb.cc.o.provides

CMakeFiles/chat.dir/session.pb.cc.o.provides.build: CMakeFiles/chat.dir/session.pb.cc.o


# Object files for target chat
chat_OBJECTS = \
"CMakeFiles/chat.dir/main.cpp.o" \
"CMakeFiles/chat.dir/InetSocketAddress.cc.o" \
"CMakeFiles/chat.dir/ServerSocket.cc.o" \
"CMakeFiles/chat.dir/Socket.cc.o" \
"CMakeFiles/chat.dir/SocketScopedBase.cc.o" \
"CMakeFiles/chat.dir/InetUtils.cc.o" \
"CMakeFiles/chat.dir/MessageUtils.cc.o" \
"CMakeFiles/chat.dir/MessageServer.cc.o" \
"CMakeFiles/chat.dir/message.pb.cc.o" \
"CMakeFiles/chat.dir/session.pb.cc.o"

# External object files for target chat
chat_EXTERNAL_OBJECTS =

chat: CMakeFiles/chat.dir/main.cpp.o
chat: CMakeFiles/chat.dir/InetSocketAddress.cc.o
chat: CMakeFiles/chat.dir/ServerSocket.cc.o
chat: CMakeFiles/chat.dir/Socket.cc.o
chat: CMakeFiles/chat.dir/SocketScopedBase.cc.o
chat: CMakeFiles/chat.dir/InetUtils.cc.o
chat: CMakeFiles/chat.dir/MessageUtils.cc.o
chat: CMakeFiles/chat.dir/MessageServer.cc.o
chat: CMakeFiles/chat.dir/message.pb.cc.o
chat: CMakeFiles/chat.dir/session.pb.cc.o
chat: CMakeFiles/chat.dir/build.make
chat: CMakeFiles/chat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable chat"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chat.dir/build: chat

.PHONY : CMakeFiles/chat.dir/build

CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/main.cpp.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/InetSocketAddress.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/ServerSocket.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/Socket.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/SocketScopedBase.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/InetUtils.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/MessageUtils.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/MessageServer.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/message.pb.cc.o.requires
CMakeFiles/chat.dir/requires: CMakeFiles/chat.dir/session.pb.cc.o.requires

.PHONY : CMakeFiles/chat.dir/requires

CMakeFiles/chat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chat.dir/clean

CMakeFiles/chat.dir/depend: message.pb.cc
CMakeFiles/chat.dir/depend: message.pb.h
CMakeFiles/chat.dir/depend: session.pb.cc
CMakeFiles/chat.dir/depend: session.pb.h
	cd /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Vadim/Documents/OS_course/hw5_client_server /Users/Vadim/Documents/OS_course/hw5_client_server /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug /Users/Vadim/Documents/OS_course/hw5_client_server/cmake-build-debug/CMakeFiles/chat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chat.dir/depend

