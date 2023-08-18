#pragma once
#include <semaphore.h>
#include <atomic>
#include <iostream>
#include <set>
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

class TcpClient;
using namespace std;
using json = nlohmann::json;
class GroupManager {
   public:
    GroupManager(int fd,
                 sem_t& rwsem,
                 atomic_bool& isGroup,
                 string& account,
                 TcpClient* tcpclient);
    ~GroupManager();
    void groupMenu();              // 群组功能主菜单
    void getGroupList();           // 获取加入群组列表
    void showGroupFunctionMenu();  // 群组
    void getNotice();
    void setChatStatus();
    void getListLen(const string groupid);
    int sendFile(int cfd, int fd, off_t offset, int size);
    void chatRequest(json& js, string permission);
    void addGroup();
    void createGroup();
    void enterGroup();
    void requiryGroup();

    void ownerMenu();
    void administratorMenu();
    void memberMenu();

    void handleOwner();
    void handleAdmin();
    void handleMember();

    void ownerChat();
    void ownerKick();
    void ownerAddAdministrator();
    void ownerRevokeAdministrator();
    void ownerCheckMember();
    void ownerCheckHistory();
    void ownerNotice();
    void ownerChangeName();
    void ownerDissolve();

    void adminChat();
    void adminKick();
    void adminCheckMember();
    void adminCheckHistory();
    void adminNotice();
    void adminExit();

    void memberChat();
    void memberCheckMember();
    void memberCheckHistory();
    void memberExit();
    void chatInput(string& data,string permission);
    void chooseFile(json& js);

    unordered_map<string, string> userGroups;  // 得到的群组列表 id+name
   private:
    string m_groupid;
    int m_fd;  // 通信fd
    sem_t& m_rwsem;
    atomic_bool& is_Group;
    string& m_account;
    TcpClient* m_tcpclient;
};