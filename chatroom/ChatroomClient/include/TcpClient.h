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
#include "GroupManager.h"

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
    void handleFriendChatRequiryResponse(const json& message);
    void handleFriendRequiryResponse(const json& message);
    void handleFriendBlockResponse(const json& message);
    void handleLoginResponse(const json& message);
    void handleRegisterResponse(const json& message);
    void handleFriendListResponse(const json& message);
    void handleFriendMsgResponse(const json& message);
    void handleFriendNoticeResponse(const json& message);

    void handleGroupListResponse(const json& message);
    void handleGroupAddResponse(const json& message);
    void handleGroupCreateResponse(const json& message);
    void handleGroupEnterResponse(const json& message);
    void handleGroupRequiryResponse(const json& message);
    void handleGroupGetNoticeResponse(const json& message);
    void handleGroupOwnerResponse(const json& message);
    void handleGroupAdminResponse(const json& message);
    void handleGroupMemberResponse(const json& message);

    void ownerChat(const json& message);
    void ownerKick(const json& message);
    void ownerAddAdministrator(const json& message);
    void ownerRevokeAdministrator(const json& message);
    void ownerCheckMember(const json& message);
    void ownerCheckHistory(const json& message);
    void ownerNotice(const json& message);
    void ownerChangeName(const json& message);
    void ownerDissolve(const json& message);

    void adminChat(const json& message);
    void adminKick(const json& message);
    void adminCheckMember(const json& message);
    void adminCheckHistory(const json& message);
    void adminNotice(const json& message);
    void adminExit(const json& message);

    void memberChat(const json& message);
    void memberCheckMember(const json& message);
    void memberCheckHistory(const json& message);
    void memberExit(const json& message);

    void handleGroupMsgResponse(const json& message);
    void handleGroupChatNoticeResponse(const json& message);
    void static addDataLen(json& js);

    void handleSetChatAckResponse(const json& message);
    void getInfo(string account);
    void handleGetListLenResponse(const json& message);
    inline string getPermisson() { return m_permission; }
    vector<string> m_groupnotice;
    int len;

   private:
    int m_fd;
    string m_permission;
    sem_t m_rwsem;  // 用于读写线程间的通信
    atomic_bool is_LoginSuccess{
        false};  // 原子类型，不需要加锁，用于记录登录状态
    atomic_bool is_Friend{
        false};  // 原子类型，不需要加锁，用于记录是否为好友状态
    atomic_bool is_Group{
        false};  // 原子类型，不需要加锁，用于记录是否为群组状态
    string m_account;
    string m_username;
    thread* m_readTask;
    FriendManager* m_friendmanager;
    GroupManager* m_groupmanager;
};