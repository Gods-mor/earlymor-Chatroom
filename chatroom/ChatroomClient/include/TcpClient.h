#pragma once
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <vector>
#include "../config/client_config.h"
#include "FriendManager.h"

using namespace std;
using json = nlohmann::json;

class TcpClient {
   public:
    TcpClient();
    ~TcpClient();
    void connectServer(char* arg1, char* arg2);
    void run();

    void welcomeMenu();  // 登录菜单
    void mainMenu();     // 主菜单
    void handleLogin();
    void handleMainMenu();
    void handleRegister();

    void readTaskHandler(int cfd);  // 子线程用于读数据
    void handleFriendAddResponse(const json& message);
    void handleFriendDeleteResponse(const json& message);
    void handleFriendChatResponse(const json& message);
    void handleFriendRequiryResponse(const json& message);
    void handleFriendBlockResponse(const json& message);
    void handleOneChatMessage(const json& message);
    void handleGroupChatMessage(const json& message);
    void handleLoginResponse(const json& message);
    void handleRegisterResponse(const json& message);
    void handleFriendListResponse(const json& message);
    void static addDataLen(json& js);

    void getInfo(string account);

   private:
    int m_fd;
    sem_t m_rwsem;  // 用于读写线程间的通信
    atomic_bool is_LoginSuccess{
        false};  // 原子类型，不需要加锁，用于记录登录状态
    string m_account;
    string m_username;
    thread* m_readTask;
    FriendManager* m_friendmanager;
};