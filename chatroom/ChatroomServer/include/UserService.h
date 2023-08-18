#pragma once
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <utility>
#include "TcpConnection.h"
#include "sw/redis++/redis++.h"  // 添加Redis++头文件

using namespace std;
using json = nlohmann::json;
constexpr int REGISTER_SUCCESS = 0;  // 注册成功
constexpr int REGISTER_FAILED = 1;   // 注册失败
constexpr int ACCOUNT_EXIST = 2;     // 帐号已注册

class UserService {
   public:
    UserService(std::shared_ptr<sw::redis::Redis> redis);

    ~UserService();
    inline void getAccount(string account) { m_account = account; }
    inline void getName(string name) { m_username = name; };
    // 是否已登录
    bool isLogin(const std::string& account);

    // 用户注册登记
    int registerUser(string account, string password, string username);

    // 登录逻辑处理
    int checkLogin(string account, string password);
    void getNotice(json requestDataJson, json& responseJson);
    void dealNotice(json requestDataJson, json& responseJson);
    
    void getList();

   private:
    // Redis* redis;
    string m_account;
    string m_username;
    std::shared_ptr<sw::redis::Redis> m_redis;
    // 与数据库交互函数属性设置为私有
};