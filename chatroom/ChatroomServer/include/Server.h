#pragma once
#include "../include/EventLoop.h"
#include "../include/ThreadPool.h"

class Server{
    public:
    Server(int port,int connections);
    // 初始化
    void run();

    // 设置监听
    void setListen();
    private:
    int m_port;
    int m_lfd;
    int m_connections;
    EventLoop* m_mainLoop;  // 指针指向主线程拥有的主线程反映实例 
	ThreadPool* m_threadPool;  // 指针保存主线程的线程池
};