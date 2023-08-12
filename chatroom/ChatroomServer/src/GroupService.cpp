#include "GroupService.h"
#include <sw/redis++/redis++.h>
#include <iostream>
#include "../config/server_config.h"
#include "OnlineUsers.h"
#include "log.h"

GroupService::GroupService(std::shared_ptr<sw::redis::Redis> redis,
                           std::shared_ptr<OnlineUsers> onlineUsersPtr)
    : m_redis(redis), m_onlineUsersPtr_(onlineUsersPtr) {
    m_onlineNumber = 0;
}
GroupService::~GroupService() {}

void GroupService::handleGetList(json requestDataJson, json& responseJson) {
    // 计算在线好友。
    getList();
    // 构建好友列表响应json
    responseJson["type"] = GROUP_LIST_ACK;
}

void GroupService::getList() {
    try {
        string key = m_account + "_Group";
        m_redis->hgetall(
            key,
            std::inserter(
                m_userGroups,
                m_userGroups.begin()));  // 获取哈希所有好友账号：昵称（keys）

    } catch (const exception& e) {
        std::cout << "get hkeys from redis error:" << e.what() << std::endl;
    }
}

void GroupService::handleGroupAdd(json requestDataJson, json& responseJson) {
     Debug("addgroup");
    cout << "addgroup" << endl;
    responseJson["grouptype"] = GROUP_ADD;
    string id = requestDataJson["groupid"];
    auto storedName = m_redis->hget(id, "groupname");

    if (!storedName) {
        responseJson["status"] = NOT_REGISTERED;
    } else {
        // 发送通知到群主或管理员处
        
    }

}

void GroupService::handleGroupCreate(json requestDataJson, json& responseJson) {
    
}

void GroupService::handleGroupRequiry(json requestDataJson,
                                      json& responseJson) {}

void GroupService::handleGroupEnter(json requestDataJson, json& responseJson) {}
