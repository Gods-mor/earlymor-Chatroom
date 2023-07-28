#pragma once
class EventLoop{
    public:
    EventLoop();
    ~EventLoop();
    
    private:
    // 该指针指向epoll实例
    EpollDispacher* m_epoll;
    
    // 任务队列（channelelement）

    
};