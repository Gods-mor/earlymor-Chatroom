#include "../include/UserService.h"
#include "log.h"
#include <sw/redis++/redis++.h>

UserService::UserService(std::shared_ptr<sw::redis::Redis> redis)
    : m_redis(redis) {}
UserService::~UserService() {}
int UserService::checkLogin(string account, string password) {

  try {
    Debug("loginUser");
    auto storedPasswordOpt = m_redis->get(account);
    std::string storedPassword;
    Debug("getpassword");
    if (storedPasswordOpt) {                      // 账号已注册
      storedPassword = storedPasswordOpt.value(); // 获取密码
    } else {
      return NOT_REGISTERED;
    }
    if (password != storedPassword) { // 密码不正确
      return WRONG_PASSWD;
    }
    if (isLogin(account)) { // 已登录
      return IS_ONLINE;
    }
    return PASS;
  } catch (const std::exception &ex) {
    std::cerr << "Error while getting password from Redis: " << ex.what()
              << std::endl;
    return ERROR;
  }
}
bool UserService::isLogin(const std::string &account) {
  try {
    auto reply = m_redis->get("status");
    return (reply && "online") ? false : true;
  } catch (const std::exception &ex) {
    std::cerr << "Error while checking login status in Redis: " << ex.what()
              << std::endl;
    return false;
  }
}
int UserService::registerUser(string account, string password,
                              string username) {

  try {
    Debug("registerUser");
    std::string storedPassword;
    auto storedPasswordOpt = m_redis->get(account);
    Debug("getpassword");
    if (storedPasswordOpt) { // 帐号已注册
      Debug("ACCOUNT_EXIST");
      return ACCOUNT_EXIST;
    }
    // 注册新用户，将帐号和密码存储到 Redis 数据库中
    m_redis->set(account, password);
    // 将用户名存储到 Redis 数据库中
    
    m_redis->hset("users", account, username);
    // 注册成功
    Debug("REGISTER_SUCCESS");
    return REGISTER_SUCCESS;
  } catch (const sw::redis::Error &ex) {
    // 捕获异常并处理
    std::cerr << "Exception occurred: " << ex.what() << std::endl;
    // 注册失败
    Debug("REGISTER_FAILED");
    return REGISTER_FAILED;
  }
}