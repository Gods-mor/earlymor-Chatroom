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
    int lfd = socket(AF_INET, SOCK_STREAM, 0);  // 监听fd
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
    ev.events = EPOLLIN;  // 默认为水平触发

    if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev)) {
        cout << "epoll_ctl fail" << endl;
        exit(1);
    }
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, HOST, &serverAddr.sin_addr);
    if (-1 == bind(lfd, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
        cout << "bind fail" << endl;
        exit(-1);
    }
    if (-1 == listen(lfd, MAX_CONNECTIONS)) {
        cout << "listen fail" << endl;
        exit(-1);
    }
    cout << "listening..." << endl;

    char ipAddress[BUFSIZ];

    while (1) {
        int nready =
            epoll_wait(epfd, events, MAX_CONNECTIONS, -1);  // 无时间限制等待
        if (nready < 0) {
            cout << "epoll_wait error" << endl;
            exit(-1);
        }
        cout << "收到" << nready << "个请求" << endl;
        for (int i = 0; i < nready; ++i) {
            int fd = events[i].data.fd;
            if (fd == lfd) {  // 新连接请求
                // 接受连接请求
                socklen_t len = sizeof(clientAddr);
                int cfd = accept(lfd, (sockaddr*)&clientAddr, &len);

                // 将cfd挂树上
                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);

                inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress,
                          sizeof(clientAddr));
                // 设置超时read
                struct timeval timeout = {1, 0};  // 超时时间设置为1s
                setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout,
                           sizeof(struct timeval));
            } else if (events[i].events & EPOLLIN) {  // 读事件
                // 接受客户端数据
                HeadData hd(fd);
                // 获取用户信息 protocolId account dataType dataLength
                unsigned int protocolId = hd.getProtocolId();
                unsigned int account = hd.getAccount();
                unsigned int dataType = hd.getDataType();
                unsigned int dataLength = hd.getDataLength();
                // 根据protocolId进行处理

                switch(protocolId){
                    case LOGIN:{ // 登录请求
                        break;
                    }
                    case REGISTER:{
                        break;
                    }
                    case CLOSE:{
                        sleep(1);
                        cout << "退出中..." << endl;
                        ev.data.fd = fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                        close(fd);
                        break;
                    }
                }
            }
        }
    }
}