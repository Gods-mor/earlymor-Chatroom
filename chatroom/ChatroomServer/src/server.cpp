#pragma once
#include "server.h"
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "log.h"
#include "tcpconnection.h"

Server::Server(int port, int connections) {
    // 初始化其他成员变量
    // ...
    m_port = port;
    m_maxconnections = connections;
    m_mainLoop = new EventLoop;
    m_threadPool = new ThreadPool(THREAD_NUMBERS);
    setListen();
}

Server::~Server() {
    // 清理资源和释放其他成员变量
    // ...
}

void Server::run() {
    Debug("服务器已经启动....");
	// 启动线程池
	m_threadPool->Run();
	// 添加检测的任务
	// 初始化channel实例
	Channel* channel = new Channel(m_lfd,FDEvent::ReadEvent,
		acceptConnection, nullptr, nullptr, this); // TODO //只要服务器还在运行监听就不会停止，
	//将当前的channel添加到任务队列里面
	m_mainLoop->AddTask(channel, ElemType::ADD);
	// 启动反应堆模型，开始工作
	m_mainLoop->Run();  //启动反应堆模型
}

void Server::setListen() {
    // 创建监听fd
    m_lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_lfd == -1) {
        perror("socket");  // 创建失败
        return;
    }
    // 设置端口复用
    int opt = 1;
    int ret;
    // 使用 setsockopt 函数来设置套接字选项，启用了端口复用
    if (-1 == setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cout << "setsockopt fail" << std::endl;
        exit(-1);
    }
    // 3，绑定端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  // IPV4协议
    addr.sin_port = htons(m_port);  // 将端口转为网络字节序 2的16次方65536个最大
    addr.sin_addr.s_addr = INADDR_ANY;  // 本地所有IP地址
    ret = bind(m_lfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");  // 失败
        return;
    }
    // 4，设置监听
    // 一次性可以和多少客户端连接,内核最大128，若给很大，内核会改为128
    ret = listen(m_lfd, MAX_CONNECTIONS);
    if (ret == -1) {
        perror("listen");
        return;
    }
}
