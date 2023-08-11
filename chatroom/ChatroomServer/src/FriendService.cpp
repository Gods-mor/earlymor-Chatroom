#include "FriendService.h"
#include <sw/redis++/redis++.h>
#include <iostream>
#include "../config/server_config.h"
#include "OnlineUsers.h"
#include "log.h"

FriendService::FriendService(std::shared_ptr<sw::redis::Redis> redis,
                             std::shared_ptr<OnlineUsers> onlineUsersPtr)
    : m_redis(redis), m_onlineUsersPtr_(onlineUsersPtr) {
    m_onlineNumber = 0;
}
FriendService::~FriendService() {}

// 从redis中找到account的好友集合
void FriendService::getList() {
    try {
        string key = m_account + "_Friend";
        m_redis->hgetall(
            key,
            std::inserter(
                m_userFriends,
                m_userFriends.begin()));  // 获取哈希所有好友账号：昵称（keys）

        m_onlineFriends.clear();
        m_offlineFriends.clear();
        // 遍历 m_userFriends 的键，与 m_onlineUsers 进行交集操作
        for (const auto& entry : m_userFriends) {
            const std::string& friendAccount = entry.first;
            if (m_onlineUsersPtr_->m_onlineUsers.find(friendAccount) !=
                m_onlineUsersPtr_->m_onlineUsers.end()) {
                m_onlineFriends.insert(entry);
            } else {
                m_offlineFriends.insert(entry);
            }
        }
        for (const std::string& user : m_onlineUsersPtr_->m_onlineUsers) {
            std::cout << user << std::endl;
        }
    } catch (const exception& e) {
        std::cout << "get hkeys from redis error:" << e.what() << std::endl;
    }
}

void FriendService::clearUnreadMsg(json& js) {
    string key = m_account + "_Friend";
    string account = js["account"];
    string jsonstr = m_redis->hget(key, account).value();
    json jsinfo = json::parse(jsonstr);

    int unreadmsg = jsinfo["unreadmsg"].get<int>();
    unreadmsg = 0;
    jsinfo["unreadmsg"] = unreadmsg;
    string chatinfo = jsinfo.dump();
    m_redis->hset(key, account, chatinfo);
}

void FriendService::handleGetList(json requestDataJson, json& responseJson) {
    // 计算在线好友。
    getList();
    // 构建好友列表响应json

    responseJson["online_friends"] = m_onlineFriends;
    responseJson["offline_friends"] = m_offlineFriends;
    responseJson["type"] = FRIEND_LIST_ACK;
}

void FriendService::handleFriend(json requestDataJson, json& responseJson) {
    cout << "requestType == FRIEND_TYPE" << endl;
    responseJson["type"] = FRIEND_ACK;
    int friendType = requestDataJson["friendtype"].get<int>();
    cout << "friendType:" << friendType << endl;
    if (friendType == FRIEND_ADD) {
        handleFriendAdd(requestDataJson, responseJson);
    }

    else if (friendType == FRIEND_DELETE) {
        handleFriendDelete(requestDataJson, responseJson);

    }

    else if (friendType == FRIEND_REQUIRY) {
        handleFriendRequiry(requestDataJson, responseJson);

    } else if (friendType == FRIEND_CHAT) {
        handleFriendChat(requestDataJson, responseJson);

    }

    else if (friendType == FRIEND_BLOCK) {
        handleFriendBlock(requestDataJson, responseJson);
    } else if (friendType == FRIEND_CHAT_REQUIRY) {
        handleFriendChatRequiry(requestDataJson, responseJson);
    }
}

void FriendService::handleFriendAdd(json requestDataJson, json& responseJson) {
    Debug("addfriend");
    cout << "addfriend" << endl;
    responseJson["friendtype"] = FRIEND_ADD;
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(account, "username");

    if (!storedName) {
        responseJson["status"] = NOT_REGISTERED;
    } else {
        json jsonvalue;
        int num = 0;
        string name = storedName.value();
        string key = m_account + "_Friend";
        string key2 = account + "_Friend";
        jsonvalue["username"] = name;
        jsonvalue["unreadmsg"] = num;
        string value = jsonvalue.dump();
        json jsonvalue2;
        jsonvalue2["username"] = m_username;
        jsonvalue2["unreadmsg"] = num;

        string value2 = jsonvalue2.dump();
        m_redis->hset(key, account, value);
        m_redis->hset(key2, m_account, value2);
        responseJson["status"] = SUCCESS_ADD_FRIEND;
    }
}

void FriendService::handleFriendDelete(json requestDataJson,
                                       json& responseJson) {
    cout << "deletefriend" << endl;
    responseJson["friendtype"] = FRIEND_DELETE;
    string key = m_account + "_Friend";
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(key, account);
    if (!storedName) {
        responseJson["status"] = NOT_FRIEND;
    } else {
        string name = storedName.value();
        m_redis->hdel(key, account);
        string key2 = account + "_Friend";
        m_redis->hdel(key2, m_account);
        responseJson["status"] = SUCCESS_DELETE_FRIEND;
    }
}

void FriendService::handleFriendRequiry(json requestDataJson,
                                        json& responseJson) {
    cout << "requiryfriend" << endl;
    responseJson["friendtype"] = FRIEND_REQUIRY;
    string key = m_account + "_Friend";
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(key, account);
    if (!storedName) {
        responseJson["status"] = NOT_FRIEND;
    } else {
        string name = storedName.value();
        responseJson["status"] = SUCCESS_REQUIRY_FRIEND;
    }
}

void FriendService::handleFriendChatRequiry(json requestDataJson,
                                            json& responseJson) {
    cout << "chatrequiryfriend" << endl;
    responseJson["friendtype"] = FRIEND_CHAT_REQUIRY;
    string key = m_account + "_Friend";
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(key, account);
    if (!storedName) {
        responseJson["status"] = NOT_FRIEND;
    } else {
        string name = storedName.value();
        responseJson["status"] = SUCCESS_REQUIRY_FRIEND;
        m_redis->hset(m_account, "chatstatus", account);
    }
}
void FriendService::handleFriendBlock(json requestDataJson,
                                      json& responseJson) {
    cout << "blockfriend" << endl;
    responseJson["friendtype"] = FRIEND_BLOCK;
    responseJson["type"] = FRIEND_ACK;
    string key = m_account + "_Friend";
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(key, account);
    if (!storedName) {
        responseJson["status"] = NOT_FRIEND;
    } else {
        string name = storedName.value();
        //
    }
}

void FriendService::handleFriendChat(json requestDataJson, json& responseJson) {
    cout << "chat with friend" << endl;
    // 构建回应json
    try {
        // 自己来自好友的未读消息清零
        clearUnreadMsg(requestDataJson);
        responseJson["friendtype"] = FRIEND_CHAT;
        responseJson["type"] = FRIEND_ACK;
        responseJson["status"] = SUCCESS_SEND_MSG;
        string receiver = requestDataJson["account"];  // 发送给谁
        string key;
        if (receiver < m_account) {
            key = receiver + "+" + m_account + "_Chat";
        } else {
            key = m_account + "+" + receiver + "_Chat";
        }
        json chatinfo;
        string data = requestDataJson["data"];
        std::time_t timestamp = std::time(nullptr);
        if (data == ":q") {
            m_redis->hset(m_account, "chatstatus", "");
            return;
        }
        string chatstatus = m_redis->hget(receiver, "chatstatus").value();
        string status = m_redis->hget(receiver, "status").value();
        if (status == "online") {
            auto connection = m_onlineUsersPtr_->getOnlineConnection(receiver);
            if (chatstatus == m_account) {  // 好友在对应聊天界面
                // 转发json数据
                // 将响应JSON数据添加到m_writeBuf中
                if (connection) {
                    json sendToFriend;
                    sendToFriend["type"] = FRIEND_MSG;
                    sendToFriend["account"] = m_account;
                    sendToFriend["username"] = m_username;
                    sendToFriend["timestamp"] = timestamp;
                    sendToFriend["data"] = data;
                    string forwardMsg = sendToFriend.dump();
                    connection->forwardMessageToFriend(forwardMsg);
                } else {
                    responseJson["status"] = FAIL_SEND_MSG;
                }
            } else {
                try {
                    string key2 =
                        receiver + "_Friend";  // 对方的来自自己未读消息加一
                    string jsonstr = m_redis->hget(key2, m_account).value();
                    json js = json::parse(jsonstr);
                    int unreadmsg = js["unreadmsg"].get<int>();
                    ++unreadmsg;
                    js["unreadmsg"] = unreadmsg;
                    string newjson = js.dump();
                    m_redis->hset(key2, m_account, newjson);
                } catch (const exception& e) {
                    cout << "chatstatus != m_account error:" << e.what()
                         << endl;
                }
                json sendToFriend;
                sendToFriend["type"] = FRIEND_NOTICE;
                sendToFriend["account"] = m_account;
                sendToFriend["username"] = m_username;
                string forwardMsg = sendToFriend.dump();
                connection->forwardMessageToFriend(forwardMsg);
            }
        }
        // 存储在数据库中

        chatinfo["account"] = m_account;  // 发送者是自己
        chatinfo["timestamp"] = timestamp;
        chatinfo["data"] = data;
        string chatmsg = chatinfo.dump();
        m_redis->rpush(key, chatmsg);
    } catch (const exception& e) {
        cout << "handleFriendChat parse json error :" << e.what() << endl;
    }
}
