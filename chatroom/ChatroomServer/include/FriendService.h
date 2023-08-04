#pragma once
#include <set>
#include <string>
class FriendService {
   public:
    FriendService();
    ~FriendService();

   private:
    int m_onlineNumber;           // 在线人数
    set<string> onlineFriends;    // 得到的在线列表
    set<string> offlineFriends;   // 得到的离线列表
    vector<string> userAFriends;  // 用户的所有好友列表
};