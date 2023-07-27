#include "epoll.h"
#include "../config/server_config.h"
#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>

Epoll::Epoll(){
    epollfd = epoll_create(MAX_CONNECTIONS); // 创建了一个 epoll 实例
    epoll_event ev{}, events[MAX_CONNECTIONS];
    ev.data.fd = ;
    ev.events = EPOLLIN;  // 默认为水平触发

    if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev)) {
        cout << "epoll_ctl fail" << endl;
        exit(1);
    }
}