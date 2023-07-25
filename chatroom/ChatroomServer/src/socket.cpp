#include "socket.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

Socket::Socket() {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);  // 监听fd
    sockaddr_in serverAddr{}, clientAddr{};
    int opt = 1;

    // 使用 setsockopt 函数来设置套接字选项，启用了端口复用
    if (-1 == setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cout << "setsockopt fail" << std::endl;
        exit(-1);
    }  // 设置端口复用
}