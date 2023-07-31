#include "UserService.h"
UserService::UserService(std::shared_ptr<sw::redis::Redis> redis)
    : m_redis(redis) {}
UserService::~UserService() {}
int UserService::checkLogin(string account, string password) {
  std::string storedPassword;

  try {
    storedPassword = m_redis->get(account);
  } catch (const std::exception &ex) {
    std::cerr << "Error while getting password from Redis: " << ex.what()
              << std::endl;
    return ERROR;
  }

  if (storedPassword.empty()) { // 帐号未注册

    return NOT_REGISTERED;
  }

  if (password != storedPassword) { // 密码不正确
    return WRONG_PASSWD;
  }
  if (isLogin(account)) { // 已登录
    return IS_ONLINE;
  }
  return PASS;
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
  std::string storedPassword;

  try {
    storedPassword = m_redis->get(account);

    if (!storedPassword.empty()) { // 帐号已注册
      return ACCOUNT_EXIST;
    }
    // 注册新用户，将帐号和密码存储到 Redis 数据库中
    m_redis->set(account, password);
    // 将用户名存储到 Redis 数据库中
    m_redis->hset("users", account, username);
    // 注册成功
    return REGISTER_SUCCESS;
  } catch (const std::exception &ex) {
    std::cerr << "Error while registering user: " << ex.what() << std::endl;
    // 注册失败
    return REGISTER_FAILED;
  }
}