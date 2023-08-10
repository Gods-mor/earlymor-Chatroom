#include "../include/UserService.h"
#include <sw/redis++/redis++.h>
#include "../config/server_config.h"
#include "log.h"

UserService::UserService(std::shared_ptr<sw::redis::Redis> redis)
    : m_redis(redis) {}
UserService::~UserService() {}
int UserService::checkLogin(string account, string password) {
    try {
        Debug("loginUser");
        string field1 = "password";
        auto storedPasswordOpt = m_redis->hget(account, field1);
        std::string storedPassword;
        Debug("getpassword");
        if (storedPasswordOpt) {                         // 账号已注册
            storedPassword = storedPasswordOpt.value();  // 获取密码
        } else {
            return NOT_REGISTERED;
        }
        if (password != storedPassword) {  // 密码不正确
            return WRONG_PASSWD;
        }
        if (isLogin(account)) {  // 已登录
            return IS_ONLINE;
        }
        return PASS;
    } catch (const std::exception& ex) {
        std::cerr << "Error while getting password from Redis: " << ex.what()
                  << std::endl;
        return ERR;
    }
}
bool UserService::isLogin(const std::string& account) {
    try {
        string field = "status";
        auto reply = m_redis->hget(account, field);
        std::string replyvalue = reply.value();
        std::string online = "online";
        return (replyvalue == online);
    } catch (const std::exception& ex) {
        std::cerr << "Error while checking login status in Redis: " << ex.what()
                  << std::endl;
        return false;
    }
}
int UserService::registerUser(string account,
                              string password,
                              string username) {
    try {
        Debug("registerUser");
        std::string storedPassword;
        auto storedPasswordOpt = m_redis->hget(account,"password");
        Debug("getpassword");
        if (storedPasswordOpt) {  // 帐号已注册
            Debug("ACCOUNT_EXIST");
            return ACCOUNT_EXIST;
        }
        std::string field1 = "username";
        std::string value1 = username;
        std::string field2 = "password";
        std::string value2 = password;
        std::string field3 = "status";
        std::string value3 = "offline";
        std::string field4 = "chatstatus";
        std::string value4 = "";
        // 将用户信息存储到 Redis 数据库中
        // 使用哈希表存储 （账号，密码，用户名，在线状态）
        m_redis->hset(account, field1, value1);
        m_redis->hset(account, field2, value2);
        m_redis->hset(account, field3, value3);
        m_redis->hset(account, field4, value4);
        // 注册成功
        Debug("REGISTER_SUCCESS");
        return REGISTER_SUCCESS;
    } catch (const sw::redis::Error& ex) {
        // 捕获异常并处理
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
        // 注册失败
        Debug("REGISTER_FAILED");
        return REGISTER_FAILED;
    }
}