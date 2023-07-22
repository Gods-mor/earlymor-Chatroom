#include <arpa/inet.h>
#include <json/json.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "config/server_config.h"

using namespace std;

int main() {
    int lfd = socket(AF_INET, SOCK_STREAM, 0); // 监听fd
    sockaddr_in serverAddr{}, clientAddr{};
    int opt = 1;
    // 使用 setsockopt 函数来设置套接字选项，启用了端口复用
    if (-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        cout << "setsockopt fail" << endl;
        exit(-1);
    }                                          // 设置端口复用
    int epfd = epoll_create(MAX_CONNECTIONS);  // 创建了一个 epoll 实例

    epoll_event ev{}, events[MAX_CONNECTIONS];
    ev.data.fd = lfd;
    ev.events = EPOLLIN ;  // 默认为水平触发
    
    if(-1 == epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev)){
        cout << "epoll_ctl fail" << endl;    
        exit(1);
    }
    cout << "listening..." << endl;

    


}