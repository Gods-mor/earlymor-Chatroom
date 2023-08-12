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
    string key = "Group_" + id;
    auto storedName = m_redis->hget(id, "groupname");
    string adkey = id + "+administrator";
    if (!storedName) {
        responseJson["status"] = NOT_REGISTERED;
    } else {
        // 发送通知到群主或管理员处
        std::unordered_set<std::string> administrator;
        // 取出管理员
        m_redis->smembers("adkey", std::back_inserter(administrator));
        for(const auto& entry :administrator){
            // 对管理员发送入群申请
            
        }
    }
}

void GroupService::handleGroupCreate(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_CREATE;
    string owner = requestDataJson["owner"];
    string groupname = requestDataJson["groupname"];
    responseJson["status"] = FAIL_CREATE_GROUP;
    /* 以下为redis分布式锁 */

    // 定义锁的键和值
    // string lockKey = "mylock";
    // string lockValue = "lockvalue";
    // try {
    //     // 获取当前时间作为锁的过期时间
    //     auto now = chrono::system_clock::now();
    //     auto expireTime = now + chrono::seconds(10);  // 10 秒过期

    //     // 尝试获取分布式锁
    //     bool acquiredLock = false;
    //     while (!acquiredLock) {
    //         acquiredLock =
    //             m_redis->set(lockKey, lockValue, "NX", "EX", 10); //
    //             设置过期时间为10秒
    //         if (!acquiredLock) {
    //             this_thread::sleep_for(
    //                 chrono::milliseconds(100));  // 等待一段时间后重新尝试
    //         }
    //     }

    //     // 成功获取锁，执行业务逻辑
    //     cout << "Acquired the lock. Performing critical section." << endl;
    //     this_thread::sleep_for(chrono::seconds(5));  // 模拟业务逻辑处理时间

    //     // 释放锁
    //     m_redis->del(lockKey);
    //     cout << "Released the lock." << endl;
    // } catch (const exception& e) {
    //     cerr << "Error: " << e.what() << endl;
    // }

    string idstr = m_redis->hget("GROUP_ID_DISPATCH", "id").value();
    int id = std::stoi(idstr);
    id++;
    string idstring = std::to_string(id);
    m_redis->hset("GROUP_ID_DISPATCH", "id", idstring);

    string key = "Group_" + idstring;
    string field1 = "owner";
    string field2 = "groupname";
    m_redis->hset(key, field1, owner);
    m_redis->hset(key, field2, groupname);
    // 把群组添加进入用户加入的群组
    string key2 = m_account + "_Group";
    m_redis->hset(key2, idstring, groupname);
    responseJson["groupid"] = id;
    responseJson["status"] = SUCCESS_CREATE_GROUP;
}

void GroupService::handleGroupRequiry(json requestDataJson,
                                      json& responseJson) {}

void GroupService::handleGroupEnter(json requestDataJson, json& responseJson) {}

void GroupService::handleGroup(json requestDataJson, json& responseJson) {
    cout << "requestType == GROUP_TYPE" << endl;
    responseJson["type"] = GROUP_ACK;
    int groupType = requestDataJson["grouptype"].get<int>();
    cout << "groupType:" << groupType << endl;
    if (groupType == GROUP_ADD) {
        handleGroupAdd(requestDataJson, responseJson);
    }

    else if (groupType == GROUP_CREATE) {
        handleGroupCreate(requestDataJson, responseJson);
    }

    else if (groupType == GROUP_ENTER) {
        handleGroupEnter(requestDataJson, responseJson);
    }

    else if (groupType == GROUP_REQUIRY) {
        handleGroupRequiry(requestDataJson, responseJson);
    }
}