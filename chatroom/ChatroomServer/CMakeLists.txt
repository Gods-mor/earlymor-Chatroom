cmake_minimum_required(VERSION 3.10)
project(ChatroomServer)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置CMAKE_PREFIX_PATH变量，包含redis++的安装目录
set(CMAKE_PREFIX_PATH "/usr/share/cmake/redis++;/usr" ${CMAKE_PREFIX_PATH})

add_executable(ChatroomServer
src/main.cpp           
src/Buffer.cpp
src/ChatService.cpp  
src/EpollDispatcher.cpp  
src/FriendService.cpp  
src/TcpServer.cpp   
src/UserService.cpp
src/Channel.cpp  
src/Dispatcher.cpp   
src/EventLoop.cpp        
src/GroupService.cpp   
src/TcpConnection.cpp  
src/ThreadPool.cpp  
src/WorkerThread.cpp
)

include_directories(/usr/include/hiredis
/include)

find_library(LINK_LIBRARIES /usr/include/hiredis)

# 添加头文件的搜索路径，例如 Redis++ 头文件路径
target_include_directories(ChatroomServer PUBLIC ${REDIS++_INCLUDE_DIRS})
target_include_directories(ChatroomServer PUBLIC /usr/include/hiredis)
# 链接 Redis++ 库
target_link_libraries(ChatroomServer PRIVATE ${REDIS++_LIBRARIES})
target_link_libraries(ChatroomServer ${LINK_LIBRARIES})