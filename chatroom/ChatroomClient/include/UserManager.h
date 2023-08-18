#pragma once
#include <semaphore.h>
#include <atomic>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class TcpClient;
using namespace std;
class UserManager {
   public:
    UserManager(int fd, sem_t& rwsem, string& account, TcpClient* tcpclient);
    ~UserManager();
    void getUserNoticeList();  // 获取好友列表
    void showUserInfo();
    void dealNotice();
    
   private:
    int m_fd;  // 通信fd
    sem_t& m_rwsem;
    string& m_account;
    TcpClient* m_tcpclient;
};