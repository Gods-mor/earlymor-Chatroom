#include "../include/Server.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

Server::Server(int port, int connections)
    : m_port(port), m_connections(connections) { // 初始化
  EventLoop* m_mainLoop = new EventLoop;
  ThreadPool* m_threadPool = new ThreadPool;
  setListen();
}

Server::~Server() {}

void Server::run() {
  // 启动线程池

  // 初始化主线程的EventLoop反应堆（Channel ）
   
  
  // 启动反应堆模型

}

void Server::setListen() {
  // 1，创建监听的fd
  // AF_INET 基于IPV4，0 流式协议中的TCP协议
  m_lfd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_lfd == -1) {
    perror("socket"); // 创建失败
    return;
  }
  // 2，设置端口复用
  int opt = 1; // 1端口复用
  int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (ret == -1) {
    perror("setsockopt"); // 创建失败
    return;
  }
  // 3，绑定端口
  struct sockaddr_in addr;
  addr.sin_family = AF_INET; // IPV4协议
  addr.sin_port = htons(m_port); // 将端口转为网络字节序 2的16次方65536个最大
  addr.sin_addr.s_addr = INADDR_ANY; // 本地所有IP地址
  ret = bind(m_lfd, (struct sockaddr *)&addr, sizeof(addr));
  if (ret == -1) {
    perror("bind"); // 失败
    return;
  }
  // 4，设置监听 一次性可以和多少客户端连接,内核最大128，若给很大，内核会改为128
  ret = listen(m_lfd, m_connections);
  if (ret == -1) {
    perror("listen");
    return;
  }
 std::cout << "listening..." << std::endl;
}