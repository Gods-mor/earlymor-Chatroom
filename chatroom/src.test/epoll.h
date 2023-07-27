#pragma once

class Epoll{
    public:
    Epoll();
    ~Epoll();
    int epollfd;
};