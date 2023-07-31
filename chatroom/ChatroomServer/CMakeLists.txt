cmake_minimum_required(VERSION 3.10)
project(ChatroomServer)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 17)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# 添加链接的库，例如 Redis++ 库
# find_package(SWRedis++ REQUIRED)
# target_link_libraries(MyExecutable ${SWRedis++_LIBRARIES})

# 添加头文件的搜索路径，例如 Redis++ 头文件路径
# include_directories(${SWRedis++_INCLUDE_DIRS})
# 添加 ChatroomServer 可执行文件的源文件
add_executable(ChatroomServer
src/Buffer.cpp
src/Channel.cpp
src/ChatService.cpp
src/Dispatcher.cpp
src/EpollDispatcher.cpp
src/EventLoop.cpp
src/FriendService.cpp
src/GroupService.cpp
    src/main.cpp
    src/TcpServer.cpp
    src/TcpConnection.cpp
    src/TcpServer.cpp
    src/ThreadPool.cpp
    src/UserService.cpp
    src/WorkerThread.cpp
)
