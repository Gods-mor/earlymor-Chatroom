#include "FriendManager.h"
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"

using namespace std;
using json = nlohmann::json;

FriendManager::FriendManager(int fd) : m_fd(fd) {
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
}

// 获取好友列表
void FriendManager::getFriendList() {
    json js;
    js["type"] = FRIEND_GET_LIST;
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getFriendList send error:" << request << endl;
    }
    
}