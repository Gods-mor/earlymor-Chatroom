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
    void resetGroupReadMsg();

    void handleGetList(json requestDataJson, json& responseJson);
    void handleGroupAdd(json requestDataJson, json& responseJson);
    void handleGroupCreate(json requestDataJson, json& responseJson);
    void handleGroupRequiry(json requestDataJson, json& responseJson);
    void handleGroupEnter(json requestDataJson, json& responseJson);

    void handleGroup(json requestDataJson, json& responseJson);

    void handleGroupOwner(json requestDataJson, json& responseJson);
    void handleGroupAdmin(json requestDataJson, json& responseJson);
    void handleGroupMember(json requestDataJson, json& responseJson);
    std::unordered_map<std::string, std::string>
        m_userGroups;  // 用户的所有群组哈希表（account：name）

    void setChatStatus(json requestDataJson, json& responseJson);

    void chat(json requestDataJson, json& responseJson, string permission);
    void ownerKick(json requestDataJson, json& responseJson);
    void ownerAddAdministrator(json requestDataJson, json& responseJson);
    void ownerRevokeAdministrator(json requestDataJson, json& responseJson);
    void ownerCheckMember(json requestDataJson, json& responseJson);
    void ownerCheckHistory(json requestDataJson, json& responseJson);
    void ownerNotice(json requestDataJson, json& responseJson);
    void ownerChangeName(json requestDataJson, json& responseJson);
    void ownerDissolve(json requestDataJson, json& responseJson);

    void adminKick(json requestDataJson, json& responseJson);
    void adminCheckMember(json requestDataJson, json& responseJson);
    void adminCheckHistory(json requestDataJson, json& responseJson);
    void adminNotice(json requestDataJson, json& responseJson);
    void adminExit(json requestDataJson, json& responseJson);

    void memberCheckMember(json requestDataJson, json& responseJson);
    void memberCheckHistory(json requestDataJson, json& responseJson);
    void memberExit(json requestDataJson, json& responseJson);

    void handleGroupGetNotice(json requestDataJson, json& responseJson);
    void getListLen(json requestDataJson, json& responseJson);
    int sendFile(int cfd, int fd, off_t offset, int size);
    string recvFile(json requestDataJson, json& responseJson, string key);
    void announce(json requestDataJson, json& responseJson, string permission);
    void announce(json requestDataJson,
                  json& responseJson,
                  string permission,
                  string filename);
    void infoRestore(json requestDataJson, string key, string permission);
    void infoRestore(string key, string permission, string filename);
    void chatResponse(json& responseJson, string permission);

   private:
    string m_account;
    string m_username;
    string m_groupid;
    int m_onlineNumber;                         // 在线人数
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
    std::shared_ptr<OnlineUsers>
        m_onlineUsersPtr_;  // 使用shared_ptr来管理onlineUsers实例
};