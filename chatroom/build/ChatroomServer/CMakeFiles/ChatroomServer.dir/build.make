# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/earlymor/文档/earlymor-Chatroom/chatroom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/earlymor/文档/earlymor-Chatroom/chatroom/build

# Include any dependencies generated for this target.
include ChatroomServer/CMakeFiles/ChatroomServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.make

# Include the progress variables for this target.
include ChatroomServer/CMakeFiles/ChatroomServer.dir/progress.make

# Include the compile flags for this target's objects.
include ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/main.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/main.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/main.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/main.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/main.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/main.cpp > CMakeFiles/ChatroomServer.dir/src/main.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/main.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/main.cpp -o CMakeFiles/ChatroomServer.dir/src/main.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Buffer.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Buffer.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Buffer.cpp > CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Buffer.cpp -o CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ChatService.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ChatService.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ChatService.cpp > CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ChatService.cpp -o CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EpollDispatcher.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EpollDispatcher.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EpollDispatcher.cpp > CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EpollDispatcher.cpp -o CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/FriendService.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/FriendService.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/FriendService.cpp > CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/FriendService.cpp -o CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpServer.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpServer.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpServer.cpp > CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpServer.cpp -o CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/UserService.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/UserService.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/UserService.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/UserService.cpp > CMakeFiles/ChatroomServer.dir/src/UserService.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/UserService.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/UserService.cpp -o CMakeFiles/ChatroomServer.dir/src/UserService.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Channel.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Channel.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/Channel.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Channel.cpp > CMakeFiles/ChatroomServer.dir/src/Channel.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/Channel.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Channel.cpp -o CMakeFiles/ChatroomServer.dir/src/Channel.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Dispatcher.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Dispatcher.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Dispatcher.cpp > CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/Dispatcher.cpp -o CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EventLoop.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EventLoop.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EventLoop.cpp > CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/EventLoop.cpp -o CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/GroupService.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/GroupService.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/GroupService.cpp > CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/GroupService.cpp -o CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpConnection.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpConnection.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpConnection.cpp > CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/TcpConnection.cpp -o CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ThreadPool.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ThreadPool.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ThreadPool.cpp > CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/ThreadPool.cpp -o CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.s

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/flags.make
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o: /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/WorkerThread.cpp
ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o: ChatroomServer/CMakeFiles/ChatroomServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o -MF CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o.d -o CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o -c /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/WorkerThread.cpp

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.i"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/WorkerThread.cpp > CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.i

ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.s"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer/src/WorkerThread.cpp -o CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.s

# Object files for target ChatroomServer
ChatroomServer_OBJECTS = \
"CMakeFiles/ChatroomServer.dir/src/main.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o" \
"CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o"

# External object files for target ChatroomServer
ChatroomServer_EXTERNAL_OBJECTS =

ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/main.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Buffer.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ChatService.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EpollDispatcher.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/FriendService.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpServer.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/UserService.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Channel.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/Dispatcher.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/EventLoop.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/GroupService.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/TcpConnection.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/ThreadPool.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/src/WorkerThread.cpp.o
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/build.make
ChatroomServer/ChatroomServer: ChatroomServer/CMakeFiles/ChatroomServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/earlymor/文档/earlymor-Chatroom/chatroom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX executable ChatroomServer"
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ChatroomServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ChatroomServer/CMakeFiles/ChatroomServer.dir/build: ChatroomServer/ChatroomServer
.PHONY : ChatroomServer/CMakeFiles/ChatroomServer.dir/build

ChatroomServer/CMakeFiles/ChatroomServer.dir/clean:
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer && $(CMAKE_COMMAND) -P CMakeFiles/ChatroomServer.dir/cmake_clean.cmake
.PHONY : ChatroomServer/CMakeFiles/ChatroomServer.dir/clean

ChatroomServer/CMakeFiles/ChatroomServer.dir/depend:
	cd /home/earlymor/文档/earlymor-Chatroom/chatroom/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/earlymor/文档/earlymor-Chatroom/chatroom /home/earlymor/文档/earlymor-Chatroom/chatroom/ChatroomServer /home/earlymor/文档/earlymor-Chatroom/chatroom/build /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer /home/earlymor/文档/earlymor-Chatroom/chatroom/build/ChatroomServer/CMakeFiles/ChatroomServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : ChatroomServer/CMakeFiles/ChatroomServer.dir/depend

