#pragma once
#include "epoll.h"
#include "threadpool.h"
#include "socket.h"

class Server{
    public:
        Server();
        Server(int port,int max_connections);
        Epoll epoll;
        Thread_pool threadpool;
        Socket socket;
}