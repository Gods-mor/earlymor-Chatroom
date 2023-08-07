#pragma once
#include <memory>  // 包含 std::shared_ptr 头文件
#include <mutex>
#include <string>         // 包含 std::string 头文件
#include <unordered_set>  // 包含 std::unordered_set 头文件
using namespace std;
class OnlineUsers {
   public:
    // 其他在线用户相关操作...
    // 添加用户到总在线用户集合
    void addOnlineUser(const std::string& userId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_onlineUsers.insert(userId);
    }

    std::unordered_set<std::string> m_onlineUsers;  // 存储总在线用户的集合
    // 静态成员函数，用于创建 OnlineUsers 对象并返回共享智能指针
    static std::shared_ptr<OnlineUsers> create() {
        return std::shared_ptr<OnlineUsers>(new OnlineUsers());
    }

   private:
    // 私有构造函数，确保只能通过 create() 函数创建 OnlineUsers 对象
    OnlineUsers() {}
    std::mutex m_mutex;  // 用于保护对 m_onlineUsers 的并发访问
};
