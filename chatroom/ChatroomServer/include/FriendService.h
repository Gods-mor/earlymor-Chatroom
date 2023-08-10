#pragma once
#include <sw/redis++/redis++.h>  // 添加Redis++头文件
#include <set>
#include <string>
#include <unordered_map>
#include "OnlineUsers.h"

// 此处添加 OnlineUsers 的前置声明
class OnlineUsers;

using namespace std;
class FriendService {
   public:
    FriendService(std::shared_ptr<sw::redis::Redis> redis,
                  std::shared_ptr<OnlineUsers> onlineUsersPtr);
    ~FriendService();
    void getList();
    inline void getAccount(string account) { m_account = account; }
    inline void addOnlineNumber() { m_onlineNumber++; }

    std::unordered_map<std::string, std::string>
        m_onlineFriends;  // 得到的在线列表
    std::unordered_map<std::string, std::string>
        m_offlineFriends;  // 得到的离线列表
    std::unordered_map<std::string, std::string>
        m_userFriends;  // 用户的所有好友哈希表（account：name）
   private:
    string m_account;
    int m_onlineNumber;                         // 在线人数
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
    std::shared_ptr<OnlineUsers>
        m_onlineUsersPtr_;  // 使用shared_ptr来管理onlineUsers实例
};