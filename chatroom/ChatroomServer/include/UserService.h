#pragma once
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <utility>
#include "sw/redis++/redis++.h"  // 添加Redis++头文件

using namespace std;

// 定义用户结构
struct User {
    std::string account;   // 用户账号
    std::string username;  // 用户名
    std::string password;  // 用户密码
};

enum Registerstatus {
    REGISTER_SUCCESS,  // 注册成功
    REGISTER_FAILED,   // 注册失败
    ACCOUNT_EXIST      // 帐号已注册
};
class UserService {
   public:
    UserService(std::shared_ptr<sw::redis::Redis> redis);

    ~UserService();

    // 是否已登录
    bool isLogin(const std::string& account);

    // 用户注册登记
    int registerUser(string account, string password, string username);

    // 登录逻辑处理
    int checkLogin(string account, string password);

   private:
    // Redis* redis;
    std::shared_ptr<sw::redis::Redis> m_redis;
    // 与数据库交互函数属性设置为私有
};