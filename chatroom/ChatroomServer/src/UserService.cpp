#include "UserService.h"
// 检查是否注册
bool UserService::isRegistered(string account) {}
// 是否已登录
bool UserService::ifLogin(string account) {
}
// 用户注册登记
  bool registerUser(){

  }
// 用户注册成功，向数据库插入数据
bool UserService::saveUser(string account, string username,
                               string password) {}
// 检查登录密码是否正确
pair<int, string> UserService::checkLogin(string account, string password) {}

// 账号是否正确
bool UserService::checkAccount(string &account) {}

// 密码是否正确
bool UserService::checkPassword(string &password) {}

UserService::UserService() {}