cmake_minimum_required(VERSION 3.10)
project(ChatroomClient)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 17)

# 包含目录

# 添加 ChatroomClient 可执行文件的源文件
add_executable(ChatroomClient
    src/main.cpp
)
target_link_libraries(ChatroomClient
    redis++ # 链接Redis++库
)