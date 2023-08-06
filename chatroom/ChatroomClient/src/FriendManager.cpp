#include "FriendManager.h"
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
#include "TcpClient.h"
using namespace std;
using json = nlohmann::json;

FriendManager::FriendManager(int fd, sem_t rwsem) : m_fd(fd), m_rwsem(rwsem) {
    ;
}

FriendManager::~FriendManager() {
    ;
}

// 好友功能菜单
void FriendManager::fiendMenu() {
    // 获取好友列表
    getFriendList();
    // 打印好友菜单
    cout << "在线好友如下：" << endl;
    for (const auto& entry : onlineFriends) {
        cout << YELLOW_COLOR << entry.second << RESET_COLOR << "("
             << entry.first << ")" << endl;
    }
    cout << "离线好友如下：" << endl;
    for (const auto& entry : offlineFriends) {
        cout << entry.second << "(" << entry.first << ")" << endl;
    }
    showFriendFunctionMenu();
    int choice;
    cin >> choice;
    switch (choice) {
        case 1:  // 添加好友
            addFriend();
            break;
        case 2:  // 删除好友
            deleteFriend();
            break;
        case 3:  // 查询好友
            queryFriend();
            break;
        case 4:  // 与好友聊天
            chatWithFriend();
            break;
        case 5:  // 拉黑好友
            blockFriend();
            break;
        default:  // 返回
            break;
    }
}

// 获取好友列表
void FriendManager::getFriendList() {
    json js;
    js["type"] = FRIEND_GET_LIST;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getFriendList send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}

void FriendManager::showFriendFunctionMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.添加好友" << endl;
    cout << "                           2.删除好友" << endl;
    cout << "                           3.查询好友 " << endl;
    cout << "                           4.与好友聊天" << endl;
    cout << "                           5.拉黑好友" << endl;
    cout << "                           6.返回" << endl;
    cout << "               #####################################" << endl;
}
void FriendManager::addFriend() {
    string account;
    cout << "请输入要添加的好友账号：";
    cin >> account;
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = FRIEND_ADD;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void FriendManager::deleteFriend() {
    string account;
    cout << "请输入要删除的好友账号：";
    cin >> account;
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = FRIEND_DELETE;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "deleteFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void FriendManager::queryFriend() {
    string account;
    cout << "请输入要查找的好友账号：";
    cin >> account;
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = FRIEND_REQUIRY;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "queryFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void FriendManager::chatWithFriend() {}
void FriendManager::blockFriend() {}