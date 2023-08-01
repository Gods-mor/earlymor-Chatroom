cmake_minimum_required(VERSION 3.10)
project(ChatroomServer)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Redis++库的安装路径
set(CMAKE_PREFIX_PATH "/usr/share/cmake/redis++;/usr" ${CMAKE_PREFIX_PATH})

# 包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/hiredis
)

# 搜索 src 目录下的源文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)

# 添加可执行文件
add_executable(ChatroomServer ${SRC_LIST})

# 添加头文件的搜索路径，例如 Redis++ 头文件路径
target_include_directories(ChatroomServer PUBLIC ${CMAKE_PREFIX_PATH})

# 链接 Redis++ 和 hiredis 库
target_link_libraries(ChatroomServer PRIVATE redis++ hiredis)
