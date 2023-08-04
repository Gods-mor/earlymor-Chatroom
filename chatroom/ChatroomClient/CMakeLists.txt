cmake_minimum_required(VERSION 3.10)
project(ChatroomClient)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include)

# 搜索 src 目录下的源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)

# 添加 ChatroomClient 可执行文件的源文件
add_executable(ChatroomClient ${SRC_LIST})

