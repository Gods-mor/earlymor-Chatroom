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
    responseJson["usergroups"] = m_userGroups;
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
    responseJson["status"] = FAIL_SEND_APPLICATION;
    Debug("addgroup");
    cout << "addgroup" << endl;
    responseJson["grouptype"] = GROUP_ADD;
    string id = requestDataJson["groupid"];
    string key = "Group_" + id;
    auto storedName = m_redis->hget(key, "groupname");
    string adkey = id + "+administrator";
    if (!storedName) {
        responseJson["status"] = NOT_REGISTERED;
    } else {
        // 发送通知到群主或管理员处
        // 只需要发送到群通知里
        /* 消息类型 type：“add“responseJson["status"] = SUCCESS_SEND_MSG;

​		来源 source：“account“

​		内容 msg：“msg”// 对方留言

​		处理者：“ad_account”

 */ string msg = requestDataJson["msg"];
        string notice = id + "_Group_Notice";
        json application;
        application["type"] = "add";
        application["source"] = m_account;
        application["msg"] = msg;
        string appstr = application.dump();
        try {
            m_redis->rpush(notice, appstr);
            responseJson["status"] = SUCCESS_SEND_APPLICATION;
        } catch (const exception& e) {
            cout << "redis rpush error:" << e.what() << endl;
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
    responseJson["groupid"] = idstring;
    responseJson["status"] = SUCCESS_CREATE_GROUP;
}

void GroupService::handleGroupRequiry(json requestDataJson,
                                      json& responseJson) {}

void GroupService::handleGroupEnter(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_ENTER;
    string groupid = requestDataJson["groupid"];
    string owner = m_redis->hget("Group_" + groupid, "owner").value();
    // 获取权限
    if (m_account == owner) {
        responseJson["permission"] = "owner";
        m_groupid = groupid;
    } else {
        // 检查元素是否存在于集合中
        bool exists = m_redis->sismember(groupid + "_Administrator", m_account);
        if (exists) {
            responseJson["permission"] = "administrator";
            m_groupid = groupid;
        } else {
            bool exists2 = m_redis->sismember(groupid + "_Member", m_account);
            if (exists2) {
                responseJson["permission"] = "member";
                m_groupid = groupid;
            } else {
                responseJson["permisson"] = "none";
                m_groupid.clear();
            }
        }
    }
}

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
    } else if (groupType == GROUP_OWNER) {
        handleGroupOwner(requestDataJson, responseJson);
    } else if (groupType == GROUP_ADMINISTRATOR) {
        handleGroupAdmin(requestDataJson, responseJson);
    } else if (groupType == GROUP_MEMBER) {
        handleGroupMember(requestDataJson, responseJson);
    }
}
void GroupService::handleGroupOwner(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_OWNER;
    int entertype = requestDataJson["entertype"];
    responseJson["entertype"] = entertype;
    if (entertype == OWNER_CHAT) {
        ownerChat(requestDataJson, responseJson);
    } else if (entertype == OWNER_KICK) {
        ownerKick(requestDataJson, responseJson);
    } else if (entertype == OWNER_ADD_ADMINISTRATOR) {
        ownerAddAdministrator(requestDataJson, responseJson);
    } else if (entertype == OWNER_REVOKE_ADMINISTRATOR) {
        ownerRevokeAdministrator(requestDataJson, responseJson);
    } else if (entertype == OWNER_CHECK_MEMBER) {
        ownerCheckMember(requestDataJson, responseJson);
    } else if (entertype == OWNER_CHECK_HISTORY) {
        ownerCheckHistory(requestDataJson, responseJson);
    } else if (entertype == OWNER_NOTICE) {
        ownerNotice(requestDataJson, responseJson);
    } else if (entertype == OWNER_CHANGE_NAME) {
        ownerChangeName(requestDataJson, responseJson);
    } else if (entertype == OWNER_DISSOLVE) {
        ownerDissolve(requestDataJson, responseJson);
    }
}
void GroupService::handleGroupAdmin(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_ADMINISTRATOR;
    int entertype = requestDataJson["entertype"];
    responseJson["entertype"] = entertype;
    if (entertype == ADMIN_CHAT) {
        adminChat(requestDataJson, responseJson);
    } else if (entertype == ADMIN_KICK) {
        adminKick(requestDataJson, responseJson);
    } else if (entertype == ADMIN_CHECK_MEMBER) {
        adminCheckMember(requestDataJson, responseJson);
    } else if (entertype == ADMIN_CHECK_HISTORY) {
        adminCheckHistory(requestDataJson, responseJson);
    } else if (entertype == ADMIN_NOTICE) {
        adminNotice(requestDataJson, responseJson);
    } else if (entertype == ADMIN_EXIT) {
        adminExit(requestDataJson, responseJson);
    }
}
void GroupService::handleGroupMember(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_MEMBER;
    int entertype = requestDataJson["entertype"];
    responseJson["entertype"] = entertype;
    if (entertype == MEMBER_CHAT) {
        memberChat(requestDataJson, responseJson);
    } else if (entertype == MEMBER_CHECK_MEMBER) {
        memberCheckMember(requestDataJson, responseJson);
    } else if (entertype == MEMBER_CHECK_HISTORY) {
        memberCheckHistory(requestDataJson, responseJson);
    } else if (entertype == MEMBER_EXIT) {
        memberExit(requestDataJson, responseJson);
    }
}
void GroupService::ownerChat(json requestDataJson, json& responseJson) {}
void GroupService::ownerKick(json requestDataJson, json& responseJson) {}
void GroupService::ownerAddAdministrator(json requestDataJson,
                                         json& responseJson) {
    string account = requestDataJson["account"];
    if (account == m_account) {
        responseJson["ownerstatus"] = NOT_SELF;
    } else {
        bool exists = m_redis->sismember(m_groupid + "_Administrator", account);
        if (exists) {
            responseJson["ownerstatus"] = ADMIN_ALREADY_EXIST;
        } else {
            bool exists2 = m_redis->sismember(m_groupid + "_Member", account);
            if (exists2) {
                m_redis->srem(m_groupid + "_Member", account);
                m_redis->sadd(m_groupid + "_Administrator", account);
                responseJson["ownerstatus"] = ADMIN_ADD_SUCCESS;
            } else {
                responseJson["ownerstatus"] = NOT_MEMBER;
            }
        }
    }
}
void GroupService::ownerRevokeAdministrator(json requestDataJson,
                                            json& responseJson) {}
void GroupService::ownerCheckMember(json requestDataJson, json& responseJson) {}
void GroupService::ownerCheckHistory(json requestDataJson, json& responseJson) {
}
void GroupService::ownerNotice(json requestDataJson, json& responseJson) {}
void GroupService::ownerChangeName(json requestDataJson, json& responseJson) {}
void GroupService::ownerDissolve(json requestDataJson, json& responseJson) {}

void GroupService::adminChat(json requestDataJson, json& responseJson) {}
void GroupService::adminKick(json requestDataJson, json& responseJson) {}
void GroupService::adminCheckMember(json requestDataJson, json& responseJson) {}
void GroupService::adminCheckHistory(json requestDataJson, json& responseJson) {
}
void GroupService::adminNotice(json requestDataJson, json& responseJson) {}
void GroupService::adminExit(json requestDataJson, json& responseJson) {}

void GroupService::memberChat(json requestDataJson, json& responseJson) {}
void GroupService::memberCheckMember(json requestDataJson, json& responseJson) {
}
void GroupService::memberCheckHistory(json requestDataJson,
                                      json& responseJson) {}
void GroupService::memberExit(json requestDataJson, json& responseJson) {}