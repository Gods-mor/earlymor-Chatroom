#pragma once

#include <sw/redis++/redis++.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <vector>
#include "eventloop.h"
#include "threadpool.h"
#include "../config/server_config.h"

using json = nlohmann::json;
using redis = sw::redis::Redis;

class Server {
   public:
    Server();
    Server(int port, int connections);
    ~Server();

    void run();
    void setListen();

   private:
    int m_maxconnections;  // 最大连接数
    int m_port;             // 监听端口
    EventLoop* m_mainLoop;  // 指针指向主线程拥有的主线程反映实例
    ThreadPool* m_threadPool;  // 指针保存主线程的线程池
    int m_lfd;                 // 服务器端用于监听的文件描述符

};
