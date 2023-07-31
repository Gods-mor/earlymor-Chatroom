cmake_minimum_required(VERSION 3.10)
project(Chatroom)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 17)

# 添加 ChatroomClient 和 ChatroomServer 的子目录
add_subdirectory(ChatroomClient)
add_subdirectory(ChatroomServer)
