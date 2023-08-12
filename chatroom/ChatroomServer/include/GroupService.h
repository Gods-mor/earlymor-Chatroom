#pragma once
#include <sw/redis++/redis++.h>  // 添加Redis++头文件
#include <set>
#include <string>
#include <unordered_map>
#include "OnlineUsers.h"

// 此处添加 OnlineUsers 的前置声明
class OnlineUsers;

using namespace std;
class GroupService {
   public:
    GroupService(std::shared_ptr<sw::redis::Redis> redis,
                 std::shared_ptr<OnlineUsers> onlineUsersPtr);
    ~GroupService();
    void getList();
    inline void getAccount(string account) { m_account = account; }
    inline void getName(string name) { m_username = name; };
    inline void addOnlineNumber() { m_onlineNumber++; }

    void handleGetList(json requestDataJson, json& responseJson);
    void handleGroupAdd(json requestDataJson, json& responseJson);
    void handleGroupCreate(json requestDataJson, json& responseJson);
    void handleGroupRequiry(json requestDataJson, json& responseJson);
    void handleGroupEnter(json requestDataJson, json& responseJson);

    void handleGroup(json requestDataJson, json& responseJson);

    std::unordered_map<std::string, std::string>
        m_userGroups;  // 用户的所有群组哈希表（account：name）
   private:
    string m_account;
    string m_username;
    int m_onlineNumber;                         // 在线人数
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
    std::shared_ptr<OnlineUsers>
        m_onlineUsersPtr_;  // 使用shared_ptr来管理onlineUsers实例
};