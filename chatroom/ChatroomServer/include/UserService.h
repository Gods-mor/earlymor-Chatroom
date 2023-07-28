#pragma once
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>

using namespace std;

class UserService {
public:
  UserService();

  ~UserService();

  // 是否已注册
  bool isRegistered(string account);
  
  // 是否已登录
  bool ifLogin(string account);
  
  // 用户注册登记
  bool registerUser();
  
  // 用户注册成功，向数据库插入数据
  bool saveUser(string account, string username, string password);
  
  // 检查登录账号密码是否正确
  pair<int, string> checkLogin(string account, string password);

private:
  // Redis* redis;

  // 与数据库交互函数属性设置为私有
  
  // 检查账号是否正确
  bool checkAccount(string &account);
  
  // 检查密码是否正确
  bool checkPassword(string &password);
};