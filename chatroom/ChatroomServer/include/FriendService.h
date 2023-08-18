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
    inline void getName(string name) { m_username = name; };
    inline void addOnlineNumber() { m_onlineNumber++; }
    string recvFile(json requestDataJson, json& responseJson, string key);
    int sendFile(int cfd, int fd, off_t offset, int size);
    void clearUnreadMsg(json& js);
    void handleGetList(json requestDataJson, json& responseJson);
    void handleFriend(json requestDataJson, json& responseJson);
    void handleFriendAdd(json requestDataJson, json& responseJson);
    void handleFriendDelete(json requestDataJson, json& responseJson);
    void handleFriendRequiry(json requestDataJson, json& responseJson);
    void handleFriendBlock(json requestDataJson, json& responseJson);
    void handleFriendChat(json requestDataJson, json& responseJson);
    void handleFriendChatRequiry(json requestDataJson, json& responseJson);

    std::unordered_map<std::string, std::string>
        m_onlineFriends;  // 得到的在线列表
    std::unordered_map<std::string, std::string>
        m_offlineFriends;  // 得到的离线列表
    std::unordered_map<std::string, std::string>
        m_userFriends;  // 用户的所有好友哈希表（account：name）
    void announce(json requestDataJson, json& responseJson);
    void announce(json requestDataJson, json& responseJson, string filename);
    void announce(string receiver, string name, string msg);
    void infoRestore(string key, json requestDataJson);
    void infoRestore(string key, string filename);

   private:
    string m_account;
    string m_username;
    int m_onlineNumber;                         // 在线人数
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
    std::shared_ptr<OnlineUsers>
        m_onlineUsersPtr_;  // 使用shared_ptr来管理onlineUsers实例
};