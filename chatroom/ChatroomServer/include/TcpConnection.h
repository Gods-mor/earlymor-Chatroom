#pragma once
#include <chrono>
#include <nlohmann/json.hpp>
#include "Buffer.h"
#include "Channel.h"
#include "ChatService.h"
#include "EventLoop.h"
#include "UserService.h"
#include "sw/redis++/redis++.h"  // 添加Redis++头文件

// OnlineUsers 和 FriendService 的前置声明
class OnlineUsers;
class FriendService;
class GroupService;
class UserService;
using json = nlohmann::json;
// 区分两者不同发送方式，
// 1,全部放到缓存区再发送，2，变放边发送
// 定义就是第一种
// 没定义即默认第二种

// 负责子线程与客户端进行通信，分别存储这读写销毁回调函数->调用相关buffer函数完成相关的通信功能
class TcpConnection {
   public:
    TcpConnection(int fd,
                  EventLoop* evloop,
                  std::shared_ptr<sw::redis::Redis> redis,
                  std::shared_ptr<OnlineUsers> onlineUsersPtr_);
    ~TcpConnection();
    static int processRead(void* arg);   // 读回调
    static int processWrite(void* arg);  // 写回调
    static int destory(void* arg);       // 销毁回调
    bool parseClientRequest(Buffer* m_readBuf);
    void getInfo();
    void setOnline();
    int getfd() { return m_fd; }
    void addDataLen(json& js);
    void static recvFile(json requestDataJson, json& responseJson, string key);
    void forwardMessageToUser(const std::string& message);
   private:
    void startHeartbeat();
    void handleEpollEvents();
    void sendHeartbeatPacket();
    void handleDataRead();
    void handleLogin(json requestDataJson, json& responseJson);
    void handleRegister(json requestDataJson, json& responseJson);
    void handleGetInfo(json requestDataJson, json& responseJson);
   
    int m_heartbeatTimerFd;  // 心跳定时器的文件描述符
    int m_fd;
    int m_epollFd;  // epoll的文件描述符
    ChatService* m_chatservice;
    UserService* m_userservice;
    FriendService* m_friendservice;
    GroupService* m_groupservice;
    string m_name;
    EventLoop* m_evLoop;
    Channel* m_channel;
    Buffer* m_readBuf;   // 读缓存区
    Buffer* m_writeBuf;  // 写缓存区
    // Client协议
    string m_username;
    string m_account;
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
    std::shared_ptr<OnlineUsers>
        m_onlineUsersPtr_;  // 使用shared_ptr来管理onlineUsers实例
};
