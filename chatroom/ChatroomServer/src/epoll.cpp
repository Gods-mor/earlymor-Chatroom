#include "epoll.h"
#include "../config/server_config.h"
#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>

Epoll::Epoll(){
    epollfd = epoll_create(MAX_CONNECTIONS); // 创建了一个 epoll 实例

}