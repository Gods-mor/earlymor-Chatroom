#include "../include/FriendService.h"
#include <sw/redis++/redis++.h>
#include <iostream>

FriendService::FriendService(std::shared_ptr<sw::redis::Redis> redis,
                             std::shared_ptr<OnlineUsers> onlineUsersPtr)
    : m_redis(redis), m_onlineUsersPtr_(onlineUsersPtr) {
    m_onlineNumber = 0;
}
FriendService::~FriendService() {}

// 从redis中找到account的好友集合
void FriendService::getList() {
    try {
        string key = m_account + "_Friend";
        m_redis->hgetall(
            key,
            std::inserter(
                m_userFriends,
                m_userFriends.begin()));  // 获取哈希所有好友账号：昵称（keys）
                
        m_onlineFriends.clear();
        m_offlineFriends.clear();
        // 遍历 m_userFriends 的键，与 m_onlineUsers 进行交集操作
        for (const auto& entry : m_userFriends) {
            const std::string& friendAccount = entry.first;
            if (m_onlineUsersPtr_->m_onlineUsers.find(friendAccount) !=
                m_onlineUsersPtr_->m_onlineUsers.end()) {
                m_onlineFriends.insert(entry);
            } else {
                m_offlineFriends.insert(entry);
            }
        }
        for (const std::string& user : m_onlineUsersPtr_->m_onlineUsers) {
            std::cout << user << std::endl;
        }
    } catch (const exception& e) {
        std::cout << "get hkeys from redis error:" << e.what() << std::endl;
    }
}