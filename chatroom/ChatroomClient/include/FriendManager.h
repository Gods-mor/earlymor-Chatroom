#pragma once
#include <iostream>
#include <set>

class FriendManager {
   public:
    FriendManager(int fd);
    ~FriendManager();
    void fiendMenu();      // 好友功能菜单
    void getFriendList();  // 获取好友列表
   private:
    int m_fd;                     // 通信fd
    int m_onlineNumber;           // 在线人数
    set<string> onlineFriends;    // 得到的在线列表
    set<string> offlineFriends;   // 得到的离线列表
    vector<string> userAFriends;  // 用户的所有好友列表
};