#pragma once
#include <set>
#include <string>
using namespace std;
class FriendService {
   public:
    FriendService(std::shared_ptr<sw::redis::Redis> redis);
    ~FriendService();
    void getList();
    inline void getAccount(string account) { m_account = account; }

   private:
    string m_account;
    int m_onlineNumber;                         // 在线人数
    set<string> onlineFriends;                  // 得到的在线列表
    set<string> offlineFriends;                 // 得到的离线列表
    vector<string> userAFriends;                // 用户的所有好友列表
    std::shared_ptr<sw::redis::Redis> m_redis;  // 使用shared_ptr来管理Redis实例
};