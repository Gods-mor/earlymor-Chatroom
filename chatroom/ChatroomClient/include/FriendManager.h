#pragma once
#include <semaphore.h>
#include <atomic>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;
class FriendManager {
   public:
    FriendManager(int fd, sem_t& rwsem,atomic_bool& isFriend,string);
    ~FriendManager();
    void fiendMenu();      // 好友功能主菜单
    void showFriendFunctionMenu();
    void addFriend();
    void deleteFriend();
    void queryFriend();
    void chatWithFriend();
    void blockFriend();
    void getFriendList();  // 获取好友列表
    unordered_map<string, string> onlineFriends;   // 得到的在线列表
    unordered_map<string, string> offlineFriends;  // 得到的离线列表
    int m_onlineNumber;                            // 在线人数
   private:
    int m_fd;  // 通信fd
    sem_t& m_rwsem;
    atomic_bool& is_Friend;
    string m_account;
    vector<string> userFriends;  // 用户的所有好友列表
};