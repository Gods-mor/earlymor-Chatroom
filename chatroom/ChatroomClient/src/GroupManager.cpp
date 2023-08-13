#include "GroupManager.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
#include "TcpClient.h"

using namespace std;
using json = nlohmann::json;

// 初始化m_fd,m_rwsem
GroupManager::GroupManager(int fd,
                           sem_t& rwsem,
                           atomic_bool& isGroup,
                           string& account,
                           TcpClient* tcpclient)
    : m_fd(fd),
      m_rwsem(rwsem),
      is_Group(isGroup),
      m_account(account),
      m_tcpclient(tcpclient) {
    // 对unordered_map进行初始化
    unordered_map<string, string> emptyMap;
    userGroups = emptyMap;
}

GroupManager::~GroupManager() {
    ;
}

// 群组主功能菜单
void GroupManager::groupMenu() {
    cout << "Groupmenu" << endl;
    while (true) {
        // system("clear");
        // 获取群组列表
        getGroupList();
        // 打印群组菜单
        sem_wait(&m_rwsem);
        showGroupFunctionMenu();
        cout << "请输入：";
        int choice;
        cin >> choice;
        cin.get();
        switch (choice) {
            case 1:  // 添加群组
                addGroup();
                break;
            case 2:  // 创建群组
                createGroup();
                break;
            case 3:  // 进入群组
                enterGroup();
                break;
            case 4:  // 查询群组
                requiryGroup();
                break;
            default:  // 返回
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}

void GroupManager::showGroupFunctionMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.添加群组" << endl;
    cout << "                           2.创建群组" << endl;
    cout << "                           3.进入群组" << endl;
    cout << "                           4.查询群组 " << endl;
    cout << "                           5.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::getGroupList() {
    cout << "getGroupList" << endl;
    json js;
    js["type"] = GROUP_GET_LIST;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getGroupList send error:" << request << endl;
    }
}
void GroupManager::addGroup() {
    string id;
    cout << "请输入要添加的群组号码：";
    cin >> id;
    cin.get();
    if (id.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        id = id.substr(0, 11);  // Truncate the input to 10 characters
    }
    string msg;
    cout << "请输入留言:";
    getline(cin, msg);
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ADD;
    js["groupid"] = id;
    js["msg"] = msg;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::createGroup() {
    string groupname;
    cout << "请输入要创建的群名称：";
    // 未来要加一些限制
    cin >> groupname;
    cin.get();
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_CREATE;
    js["owner"] = m_account;
    js["groupname"] = groupname;
    // 群组id由服务器分配
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::enterGroup() {
    string id;
    cout << "请输入要进入的群组号码：";
    cin >> id;
    cin.get();
    if (id.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        id = id.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ENTER;
    js["groupid"] = id;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "enterGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
    string permisson = m_tcpclient->getPermisson();
    if (permisson == "owner") {
        ownerMenu();
        handleOwner();
    } else if (permisson == "administrator") {
        administratorMenu();
        handleAdmin();
    } else {
        memberMenu();
        handleMember();
    }
}
void GroupManager::ownerMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.踢人" << endl;
    cout << "                           3.添加管理员" << endl;
    cout << "                           4.查看群成员 " << endl;
    cout << "                           5.查看历史记录" << endl;
    cout << "                           6.群通知" << endl;
    cout << "                           7.修改群名" << endl;
    cout << "                           8.解散该群" << endl;
    cout << "                           9.返回" << endl;
    cout << "               #####################################" << endl;
}

void GroupManager::administratorMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.踢人" << endl;
    cout << "                           3.查看群成员" << endl;
    cout << "                           4.查看历史记录 " << endl;
    cout << "                           5.群通知" << endl;
    cout << "                           6.退出该群" << endl;
    cout << "                           7.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::memberMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.查看群成员" << endl;
    cout << "                           3.查看历史记录" << endl;
    cout << "                           4.退出该群 " << endl;
    cout << "                           5.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::handleOwner(){
    
}

void GroupManager::handleAdmin(){}

void GroupManager::handleMember(){}

void GroupManager::requiryGroup() {}
