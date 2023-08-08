#include "FriendManager.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
#include "TcpClient.h"

using namespace std;
using json = nlohmann::json;

// 初始化m_fd,m_rwsem
FriendManager::FriendManager(int fd, sem_t& rwsem, atomic_bool& isFriend)
    : m_fd(fd), m_rwsem(rwsem), is_Friend(isFriend) {
    // 对unordered_map进行初始化
    unordered_map<string, string> emptyMap;
    onlineFriends = emptyMap;  // 将emptyMap赋值给onlineFriends
    offlineFriends = emptyMap;
    // 对vector进行初始化
    vector<string> emptyVector;
    userFriends = emptyVector;  // 将emptyVector赋值给userFriends
}

FriendManager::~FriendManager() {
    ;
}

// 好友主功能菜单
void FriendManager::fiendMenu() {
    cout << "friendmenu" << endl;
    while (true) {
        sleep(1);
        // system("clear");
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
        cout << "请输入：";
        int choice;
        cin >> choice;
        cin.get();
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
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}

// 获取好友列表
void FriendManager::getFriendList() {
    cout << "getFriendList" << endl;
    json js;
    js["type"] = FRIEND_GET_LIST;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getFriendList send error:" << request << endl;
    }
    cout << "sem_wait" << endl;
    sem_wait(&m_rwsem);
    // cout << "sem_post" << endl;
}

// 好友功能选择菜单
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

// 添加好友，通过账号添加
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
    js["type"] = FRIEND_TYPE;
    js["friendtype"] = FRIEND_ADD;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}

// 删除好友，通过账号删除
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
    js["type"] = FRIEND_TYPE;
    js["friendtype"] = FRIEND_DELETE;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "deleteFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}

// 查找好友，通过账号查找
void FriendManager::queryFriend() {
    string account;
    cout << "请输入好友账号：";
    cin >> account;
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = FRIEND_TYPE;
    js["friendtype"] = FRIEND_REQUIRY;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "queryFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}

// 和好友聊天
void FriendManager::chatWithFriend() {
    string account;
    cout << "请输入好友账号：";
    cin >> account;
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = FRIEND_TYPE;
    js["friendtype"] = FRIEND_REQUIRY;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "queryFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
    // 以上为检验是否为好友，以下是聊天消息发送
    if (!is_Friend) {
        return;
    } else {
        while (true) {
            js["type"] = FRIEND_TYPE;
            js["friendtype"] = FRIEND_CHAT;
            js["account"] = account;
            string data;
            getline(cin, data);
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
        }
    }
}

// 拉黑好友
void FriendManager::blockFriend() {
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
    js["type"] = FRIEND_TYPE;
    js["friendtype"] = FRIEND_BLOCK;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "queryFriend send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}