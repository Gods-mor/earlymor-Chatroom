#include "GroupService.h"
#include <sw/redis++/redis++.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <fstream>
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
    responseJson["type"] = GROUP_GET_LIST;
    responseJson["usergroups"] = m_userGroups;
}

void GroupService::getList() {
    try {
        string key = m_account + "_Group";
        m_userGroups.clear();
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
    json jsonmsg;
    jsonmsg["groupname"] = groupname;
    jsonmsg["readmsg"] = 0;
    string groupmsg = jsonmsg.dump();
    m_redis->hset(key2, idstring, groupmsg);
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
    responseJson["type"] = GROUP_TYPE;
    int groupType = requestDataJson["grouptype"].get<int>();
    responseJson["grouptype"] = groupType;
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
    } else if (groupType == GROUP_GET_NOTICE) {
        handleGroupGetNotice(requestDataJson, responseJson);
    }
}
void GroupService::handleGroupOwner(json requestDataJson, json& responseJson) {
    responseJson["grouptype"] = GROUP_OWNER;
    int entertype = requestDataJson["entertype"];
    responseJson["entertype"] = entertype;
    if (entertype == OWNER_CHAT) {
        chat(requestDataJson, responseJson, "owner");
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
        chat(requestDataJson, responseJson, "administrator");
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
        chat(requestDataJson, responseJson, "member");
    } else if (entertype == MEMBER_CHECK_MEMBER) {
        memberCheckMember(requestDataJson, responseJson);
    } else if (entertype == MEMBER_CHECK_HISTORY) {
        memberCheckHistory(requestDataJson, responseJson);
    } else if (entertype == MEMBER_EXIT) {
        memberExit(requestDataJson, responseJson);
    }
}
// 聊天
void GroupService::chat(json requestDataJson,
                        json& responseJson,
                        string permission) {
    try {
        // 自己来自群聊的已读消息序列置为最新消息的序列
        resetGroupReadMsg();
        chatResponse(responseJson, permission);
        string data = requestDataJson["data"];
        json chatinfo;
        string key = m_groupid + "_Chat";
        if (data == ":q") {
            m_redis->hset(m_account, "chatstatus", "");
        } else if (data == ":h") {
            std::vector<std::string> msg;
            m_redis->lrange(key, 0, -1, std::back_inserter(msg));
            responseJson["msg"] = msg;
            responseJson["status"] = GET_GROUP_HISTORY;
        } else if (data == ":f") {
            // 接受发文件的请求
            string filename = recvFile(requestDataJson, responseJson, key);
            announce(requestDataJson, responseJson, permission, filename);
            infoRestore(key, permission, filename);
        } else if (data == ":r") {
            struct stat info;
            responseJson["status"] = ACCESS_FILE_FAIL;
            if (stat(key.c_str(), &info) != 0) {
                // 目录不存在，返回no file
                responseJson["status"] = NO_FILE;
                return;
            } else {
                // 打开目录
                DIR* directory = opendir(key.c_str());
                if (!directory) {
                    perror("opendir");
                    return;
                }
                // 遍历目录中的文件
                vector<string> fileList;
                struct dirent* entry;
                while ((entry = readdir(directory)) != nullptr) {
                    if (entry->d_type == DT_REG) {
                        // d_name 存储文件名
                        fileList.push_back(entry->d_name);
                    }
                }
                responseJson["status"] = FILE_LIST;
                responseJson["filelist"] = fileList;
                // 关闭目录
                closedir(directory);
                TcpConnection* connection =
                    m_onlineUsersPtr_->getOnlineConnection(m_account);
                string message = responseJson.dump();
                int n = send(connection->getfd(), message.c_str(),
                             strlen(message.c_str()) + 1, 0);
                if (n == -1) {
                    perror("send");
                }
                cout << "send filelist" << endl;
                char buffer[4096];
                n = recv(connection->getfd(), buffer, 4096, 0);
                if (n == 0 || n == -1) {
                    perror("recv");
                    return;
                }
                json js = json::parse(buffer);
                string filename = js["filename"];
                cout << "recv" << filename << endl;
                string filepath = key + "/" + filename;
                int fd = open(filepath.c_str(), O_RDONLY);
                if (fd == -1) {
                    perror("open");
                    cout << "发送失败，返回" << endl;
                    return;
                }
                // 获取文件大小
                struct stat file_stat;
                if (fstat(fd, &file_stat) == -1) {
                    perror("fstat");
                    close(fd);
                    cout << "发送失败，返回" << endl;
                    return;
                }

                responseJson["status"] = GET_FILE_SIZE;
                responseJson["filesize"] = file_stat.st_size;
                responseJson["filename"] = filename;
                string request = responseJson.dump();
                int len = send(connection->getfd(), request.c_str(),
                               strlen(request.c_str()) + 1, 0);
                if (0 == len || -1 == len) {
                    cerr << "data send error:" << request << endl;
                }
                cout << "send file size" << endl;
                usleep(10);
                off_t offset = 0;  // 从文件开始位置开始发送
                size_t remaining_bytes =
                    file_stat.st_size;  // 剩余要发送的字节数
                int sum =
                    sendFile(connection->getfd(), fd, offset, remaining_bytes);
                if (sum == 0 || sum == -1) {
                    cout << "sendfile error" << endl;
                }
                // cout << "sendFile" << endl;
                close(fd);
                responseJson["status"] = ACCESS_FILE_SUCCESS;
            }
        } else {
            announce(requestDataJson, responseJson, permission);
            // 存储在数据库中
            infoRestore(requestDataJson, key, permission);
        }

    } catch (const exception& e) {
        cout << "ownerChat parse json error :" << e.what() << endl;
    }
}

void GroupService::ownerKick(json requestDataJson, json& responseJson) {
    string account = requestDataJson["account"];
    // 判断是否有该成员
    bool exists = m_redis->sismember(m_groupid + "_Member", account);
    responseJson["status"] = FAIL_TO_KICK;
    if (exists) {
        m_redis->hdel(account + "_Group", m_groupid);
        m_redis->srem(m_groupid + "_Member", account);
        responseJson["status"] = SUCCESS_KICK;
    } else {
        bool exists2 =
            m_redis->sismember(m_groupid + "_Administrator", account);

        if (exists2) {
            m_redis->srem(m_groupid + "_Administrator", account);
            m_redis->hdel(account + "_Group", m_groupid);
            responseJson["status"] = SUCCESS_KICK;
        }
    }
}
// 添加管理员
void GroupService::ownerAddAdministrator(json requestDataJson,
                                         json& responseJson) {
    string account = requestDataJson["account"];
    if (account == m_account) {
        responseJson["status"] = NOT_SELF;
    } else {
        bool exists = m_redis->sismember(m_groupid + "_Administrator", account);
        if (exists) {
            responseJson["status"] = ADMIN_ALREADY_EXIST;
        } else {
            bool exists2 = m_redis->sismember(m_groupid + "_Member", account);
            if (exists2) {
                m_redis->srem(m_groupid + "_Member", account);
                m_redis->sadd(m_groupid + "_Administrator", account);
                responseJson["status"] = ADMIN_ADD_SUCCESS;
                json info;
                info["type"] = "promote";
                info["source"] = m_account;
                info["member"] = account;
                string infostr = info.dump();
                string key = m_groupid + "_Group_Notice";
                m_redis->rpush(key, infostr);
            } else {
                responseJson["status"] = NOT_MEMBER;
            }
        }
    }
}
// 撤除管理员
void GroupService::ownerRevokeAdministrator(json requestDataJson,
                                            json& responseJson) {
    string account = requestDataJson["account"];
    if (account == m_account) {
        responseJson["status"] = NOT_SELF;
    } else {
        bool exists = m_redis->sismember(m_groupid + "_Administrator", account);
        if (exists) {
            m_redis->srem(m_groupid + "_Administrator", account);
            m_redis->sadd(m_groupid + "_Member", account);
            responseJson["status"] = SUCCESS_REVOKE_ADMIN;
            string key = m_groupid + "_Group_Notice";
            json info;
            info["type"] = "revoke";
            info["dealer"] = m_account;
            info["member"] = account;
            string infostr = info.dump();
            m_redis->rpush(key, infostr);
        } else {
            responseJson["status"] = NOT_MEMBER;
        }
    }
}

void GroupService::ownerCheckMember(json requestDataJson, json& responseJson) {
    responseJson["entertype"] = OWNER_CHECK_MEMBER;
    responseJson["grouptype"] = GROUP_OWNER;
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = FAIL_TO_CHECK;
    string owner = m_redis->hget("Group_" + m_groupid, "owner").value();
    responseJson["owner"] = owner;
    unordered_set<string> admin;
    m_redis->smembers(m_groupid + "_Administrator",
                      std::inserter(admin, admin.begin()));
    responseJson["administrator"] = admin;
    unordered_set<string> member;
    m_redis->smembers(m_groupid + "_Member",
                      std::inserter(member, member.begin()));
    responseJson["member"] = member;
    responseJson["status"] = SUCCESS_CHECK;
}
void GroupService::ownerCheckHistory(json requestDataJson, json& responseJson) {
}
void GroupService::ownerNotice(json requestDataJson, json& responseJson) {
    string key = m_groupid + "_Group_Notice";
    int number = requestDataJson["number"].get<int>();
    string msg = m_redis->lindex(key, number).value();
    json info;
    info = json::parse(msg);
    string account = info["source"];
    string choice = requestDataJson["choice"];
    info["dealer"] = m_account;
    info["result"] = choice;
    string infostr = info.dump();
    string groupname = m_redis->hget("Group_" + m_groupid, "groupname").value();
    json memgroup;
    memgroup["groupname"] = groupname;
    memgroup["readmsg"] = 0;
    string memgroupstr = memgroup.dump();
    string groupkey = m_groupid + "_Member";
    if (choice == "accept") {
        m_redis->sadd(groupkey, account);
        m_redis->lset(key, number, infostr);
        m_redis->hset(account + "_Group", m_groupid, memgroupstr);
        responseJson["status"] = SUCCESS_ACCEPT_MEMBER;
    } else if (choice == "refuse") {
        m_redis->lset(key, number, infostr);
        responseJson["status"] = SUCCESS_REFUSE_MEMBER;
    } else {
        responseJson["status"] = FAIL_DEAL_MEMBER;
    }
}
void GroupService::ownerChangeName(json requestDataJson, json& responseJson) {}
void GroupService::ownerDissolve(json requestDataJson, json& responseJson) {
    unordered_set<string> groupMember;
    m_redis->smembers(m_groupid + "_Member",
                      std::inserter(groupMember, groupMember.begin()));

    unordered_set<string> groupAdministrator;
    m_redis->smembers(
        m_groupid + "_Administrator",
        std::inserter(groupAdministrator, groupAdministrator.begin()));
    // 成员的group表（hash）account+"_Group" 将群聊id对应消息清除
    for (const auto& entry : groupMember) {
        string key = entry + "_Group";
        m_redis->hdel(key, m_groupid);
    }
    // ​管理员的group表（hash）account+"_Group"
    // 将群聊id对应消息清除
    for (const auto& entry : groupAdministrator) {
        string key = entry + "_Group";
        m_redis->hdel(key, m_groupid);
    }

    // ​群组成员表(set)   id+"_Member"
    m_redis->del(m_groupid + "_Member");
    // ​群组管理员表(set) id+"_Administrator"
    m_redis->del(m_groupid + "_Administrator");
    // ​群组hash表(hash)  "Group_"+id
    m_redis->del("Group_" + m_groupid);
    // ​群组聊天表(list)  id+"_Chat"
    m_redis->del(m_groupid + "_Chat");
    // ​群组通知表(list)  id +"_Group_Notice"
    m_redis->del(m_groupid + "_Group_Notice");
    // ​群主的group表（hash）account+"_Group"
    // 将群聊id对应消息清除
    string key = m_account + "_Group";
    m_redis->hdel(key, m_groupid);

    responseJson["status"] = DISSOLVE_SUCCESS;
}

void GroupService::adminKick(json requestDataJson, json& responseJson) {
    string account = requestDataJson["account"];
    // 判断是否有该成员
    bool exists = m_redis->sismember(m_groupid + "_Member", account);
    responseJson["status"] = FAIL_TO_KICK;
    if (exists) {
        m_redis->hdel(account + "_Group", m_groupid);
        m_redis->srem(m_groupid + "_Member", account);
        responseJson["status"] = SUCCESS_KICK;
    }
}
void GroupService::adminCheckMember(json requestDataJson, json& responseJson) {
    responseJson["entertype"] = ADMIN_CHECK_MEMBER;
    responseJson["grouptype"] = GROUP_ADMINISTRATOR;
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = FAIL_TO_CHECK;
    string owner = m_redis->hget("Group_" + m_groupid, "owner").value();
    responseJson["owner"] = owner;
    unordered_set<string> admin;
    m_redis->smembers(m_groupid + "_Administrator",
                      std::inserter(admin, admin.begin()));
    responseJson["administrator"] = admin;
    unordered_set<string> member;
    m_redis->smembers(m_groupid + "_Member",
                      std::inserter(member, member.begin()));
    responseJson["member"] = member;
    responseJson["status"] = SUCCESS_CHECK;
}
void GroupService::adminCheckHistory(json requestDataJson, json& responseJson) {
}
void GroupService::adminNotice(json requestDataJson, json& responseJson) {
    string key = m_groupid + "_Group_Notice";
    int number = requestDataJson["number"].get<int>();
    string msg = m_redis->lindex(key, number).value();
    json info;
    info = json::parse(msg);
    string account = info["source"];
    string choice = requestDataJson["choice"];
    info["dealer"] = m_account;
    info["result"] = choice;
    string infostr = info.dump();
    string groupname = m_redis->hget("Group_" + m_groupid, "groupname").value();
    json memgroup;
    memgroup["groupname"] = groupname;
    memgroup["readmsg"] = 0;
    string memgroupstr = memgroup.dump();
    string groupkey = m_groupid + "_Member";
    if (choice == "accept") {
        m_redis->sadd(groupkey, account);
        m_redis->lset(key, number, infostr);
        m_redis->hset(account + "_Group", m_groupid, memgroupstr);
        responseJson["status"] = SUCCESS_ACCEPT_MEMBER;
    } else if (choice == "refuse") {
        m_redis->lset(key, number, infostr);
        responseJson["status"] = SUCCESS_REFUSE_MEMBER;
    } else {
        responseJson["status"] = FAIL_DEAL_MEMBER;
    }
}
void GroupService::adminExit(json requestDataJson, json& responseJson) {
    responseJson["entertype"] = ADMIN_EXIT;
    responseJson["grouptype"] = GROUP_ADMINISTRATOR;
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = FAIL_TO_EXIT;
    // 成员的group表（hash）account+"_Group" 将群聊id对应消息清除
    m_redis->hdel(m_account + "_Group", m_groupid);
    // 群组成员表(set)      id+"_Member"
    m_redis->srem(m_groupid + "_Administrator", m_account);
    responseJson["status"] = SUCCESS_EXIT;
}

void GroupService::memberCheckMember(json requestDataJson, json& responseJson) {
    responseJson["entertype"] = MEMBER_CHECK_MEMBER;
    responseJson["grouptype"] = GROUP_MEMBER;
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = FAIL_TO_CHECK;
    string owner = m_redis->hget("Group_" + m_groupid, "owner").value();
    responseJson["owner"] = owner;
    unordered_set<string> admin;
    m_redis->smembers(m_groupid + "_Administrator",
                      std::inserter(admin, admin.begin()));
    responseJson["administrator"] = admin;
    unordered_set<string> member;
    m_redis->smembers(m_groupid + "_Member",
                      std::inserter(member, member.begin()));
    responseJson["member"] = member;
    responseJson["status"] = SUCCESS_CHECK;
}
void GroupService::memberCheckHistory(json requestDataJson,
                                      json& responseJson) {}
void GroupService::memberExit(json requestDataJson, json& responseJson) {
    responseJson["entertype"] = MEMBER_EXIT;
    responseJson["grouptype"] = GROUP_MEMBER;
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = FAIL_TO_EXIT;
    // 成员的group表（hash）account+"_Group" 将群聊id对应消息清除
    m_redis->hdel(m_account + "_Group", m_groupid);
    // 群组成员表(set)      id+"_Member"
    m_redis->srem(m_groupid + "_Member", m_account);
    responseJson["status"] = SUCCESS_EXIT;
}
void GroupService::handleGroupGetNotice(json requestDataJson,
                                        json& responseJson) {
    string id = requestDataJson["groupid"];
    // 使用 LRANGE 命令获取队列中的所有元素
    string key = id + "_Group_Notice";
    std::vector<std::string> msg;
    m_redis->lrange(key, 0, -1, std::back_inserter(msg));
    responseJson["notice"] = msg;
    responseJson["grouptype"] = GROUP_GET_NOTICE;
}

void GroupService::resetGroupReadMsg() {
    string key = m_account + "_Group";
    string field = m_groupid;
    string info = m_redis->hget(key, field).value();
    json infojs = json::parse(info);
    int readmsgcnt = m_redis->llen(m_groupid + "_Chat");
    infojs["readmsg"] = readmsgcnt;
    string msg = infojs.dump();
    m_redis->hset(key, field, msg);
}

void GroupService::setChatStatus(json requestDataJson, json& responseJson) {
    string groupid = requestDataJson["groupid"];

    m_redis->hset(m_account, "chatstatus", groupid);
    responseJson["status"] = SUCCESS_SET_CHATSTATUS;
    responseJson["type"] = GROUP_SET_CHAT_STATUS;
}

void GroupService::getListLen(json requestDataJson, json& responseJson) {
    string groupid = requestDataJson["groupid"];
    int len = m_redis->llen(groupid + "_Chat");
    responseJson["type"] = GROUP_GET_LIST_LEN;
    responseJson["len"] = len;
}

string GroupService::recvFile(json requestDataJson,
                              json& responseJson,
                              string key) {
    string filepath = requestDataJson["filepath"];
    size_t filesize = requestDataJson["filesize"];
    // 使用 C++17 标准库的 filesystem 来获取文件名
    std::filesystem::path path(filepath);
    std::string filename = path.filename().string();
    responseJson["status"] = ALREADY_TO_FILE;
    string forwardMsg = responseJson.dump();
    // 将响应JSON数据添加到m_writeBuf中
    TcpConnection* connection =
        m_onlineUsersPtr_->getOnlineConnection(m_account);
    int n = send(connection->getfd(), forwardMsg.c_str(),
                 strlen(forwardMsg.c_str()) + 1, 0);
    // 创建特定文件夹（如果不存在）
    if (access(key.c_str(), F_OK) == -1) {
        mkdir(key.c_str(), 0777);
    }
    // 拼接文件路径
    std::string file_path = key + "/" + filename;

    // 创建文件来保存接收的数据
    std::ofstream received_file(file_path, std::ios::binary);

    // 接收文件数据并写入文件
    char buffer[4096];
    ssize_t bytes_received;
    cout << "---------start recv----------" << connection->getfd() << endl;
    size_t sum = 0;
    while (sum < filesize) {
        bytes_received = recv(connection->getfd(), buffer, sizeof(buffer), 0);
        cout << "recv----------" << bytes_received << "字节" << endl;
        received_file.write(buffer, bytes_received);
        sum += bytes_received;
    }
    cout << "recv sum " << sum << "字节" << endl;
    responseJson["status"] = SUCCESS_RECV_FILE;
    // 关闭套接字和文件
    received_file.close();
    return filename;
}
void GroupService::announce(json requestDataJson,
                            json& responseJson,
                            string permission) {
    string set1 = m_groupid + "_Member";
    string set2 = m_groupid + "_Administrator";
    string data = requestDataJson["data"];
    std::time_t timestamp = std::time(nullptr);
    string online_users = "ONLINE_USERS";
    // 获取在线群成员的集合,遍历得到chatstatus
    unordered_set<string> groupMemberOnline;
    m_redis->sinter(
        {online_users, set1},
        std::inserter(groupMemberOnline, groupMemberOnline.begin()));
    m_redis->sinter({online_users, set2},
                    std::inserter(groupMemberOnline, groupMemberOnline.end()));
    // 加上群主
    string owner = m_redis->hget("Group_" + m_groupid, "owner").value();
    bool exist = m_redis->sismember(online_users, owner);
    if (exist) {
        groupMemberOnline.insert(owner);
    }
    for (const auto& entry : groupMemberOnline) {
        if (entry == m_account) {
            continue;
        }
        string chatstatus = m_redis->hget(entry, "chatstatus").value();
        string status = m_redis->hget(entry, "status").value();

        auto connection = m_onlineUsersPtr_->getOnlineConnection(entry);
        if (chatstatus == m_groupid) {  // 群成员在对应聊天界面
            // 转发json数据
            // 将响应JSON数据添加到m_writeBuf中
            if (connection) {
                json sendToMember;
                sendToMember["type"] = GROUP_MSG;
                sendToMember["account"] = m_account;
                sendToMember["username"] = m_username;
                sendToMember["timestamp"] = timestamp;
                sendToMember["permission"] = permission;
                sendToMember["data"] = data;
                string forwardMsg = sendToMember.dump();
                connection->forwardMessageToUser(forwardMsg);
            } else {
                responseJson["status"] = FAIL_SEND_MSG;
            }
        } else {
            json sendToMember;
            sendToMember["type"] = GROUP_CHAT_NOTICE;
            sendToMember["id"] = m_groupid;
            string groupname =
                m_redis->hget("Group_" + m_groupid, "groupname").value();
            sendToMember["groupname"] = groupname;
            string forwardMsg = sendToMember.dump();
            connection->forwardMessageToUser(forwardMsg);
        }
    }
}
void GroupService::announce(json requestDataJson,
                            json& responseJson,
                            string permission,
                            string filename) {
    string set1 = m_groupid + "_Member";
    string set2 = m_groupid + "_Administrator";
    string data =
        "->" + permission + ":" + m_account + "发送了文件" + filename + "<-";
    std::time_t timestamp = std::time(nullptr);
    string online_users = "ONLINE_USERS";
    // 获取在线群成员的集合,遍历得到chatstatus
    unordered_set<string> groupMemberOnline;
    m_redis->sinter(
        {online_users, set1},
        std::inserter(groupMemberOnline, groupMemberOnline.begin()));
    m_redis->sinter({online_users, set2},
                    std::inserter(groupMemberOnline, groupMemberOnline.end()));
    // 加上群主
    string owner = m_redis->hget("Group_" + m_groupid, "owner").value();
    bool exist = m_redis->sismember(online_users, owner);
    if (exist) {
        groupMemberOnline.insert(owner);
    }
    for (const auto& entry : groupMemberOnline) {
        if (entry == m_account) {
            continue;
        }
        string chatstatus = m_redis->hget(entry, "chatstatus").value();
        string status = m_redis->hget(entry, "status").value();

        auto connection = m_onlineUsersPtr_->getOnlineConnection(entry);
        if (chatstatus == m_groupid) {  // 群成员在对应聊天界面
            // 转发json数据
            // 将响应JSON数据添加到m_writeBuf中
            if (connection) {
                json sendToMember;
                sendToMember["type"] = GROUP_MSG;
                sendToMember["account"] = m_account;
                sendToMember["username"] = m_username;
                sendToMember["timestamp"] = timestamp;
                sendToMember["permission"] = permission;
                sendToMember["data"] = data;
                string forwardMsg = sendToMember.dump();
                connection->forwardMessageToUser(forwardMsg);
            } else {
                responseJson["status"] = FAIL_SEND_MSG;
            }
        } else {
            json sendToMember;
            sendToMember["type"] = GROUP_CHAT_NOTICE;
            sendToMember["id"] = m_groupid;
            string groupname =
                m_redis->hget("Group_" + m_groupid, "groupname").value();
            sendToMember["groupname"] = groupname;
            string forwardMsg = sendToMember.dump();
            connection->forwardMessageToUser(forwardMsg);
        }
    }
}

void GroupService::infoRestore(json requestDataJson,
                               string key,
                               string permission) {
    json chatinfo;
    string data = requestDataJson["data"];
    std::time_t timestamp = std::time(nullptr);
    // 存储在数据库中
    chatinfo["account"] = m_account;  // 发送者是自己
    chatinfo["permission"] = permission;
    chatinfo["timestamp"] = timestamp;
    chatinfo["data"] = data;
    string chatmsg = chatinfo.dump();
    m_redis->rpush(key, chatmsg);
}
void GroupService::infoRestore(string key, string permission, string filename) {
    json chatinfo;
    std::time_t timestamp = std::time(nullptr);
    string data =
        "->" + permission + ":" + m_account + "发送了文件" + filename + "<-";
    // 存储在数据库中
    chatinfo["account"] = m_account;  // 发送者是自己
    chatinfo["permission"] = permission;
    chatinfo["timestamp"] = timestamp;
    chatinfo["data"] = data;
    string chatmsg = chatinfo.dump();
    m_redis->rpush(key, chatmsg);
}

void GroupService::chatResponse(json& responseJson, string permission) {
    responseJson["type"] = GROUP_TYPE;
    responseJson["status"] = SUCCESS_SEND_MSG;
    if (permission == "owner") {
        responseJson["entertype"] = OWNER_CHAT;
        responseJson["grouptype"] = GROUP_OWNER;
    } else if (permission == "administrator") {
        responseJson["entertype"] = ADMIN_CHAT;
        responseJson["grouptype"] = GROUP_ADMINISTRATOR;
    } else if (permission == "member") {
        responseJson["entertype"] = MEMBER_CHAT;
        responseJson["grouptype"] = GROUP_MEMBER;
    }
}
int GroupService::sendFile(int cfd, int fd, off_t offset, int size) {
    int count = 0;
    while (offset < size) {
        // 系统函数，发送文件，linux内核提供的sendfile 也能减少拷贝次数
        //  sendfile发送文件效率高，而文件目录使用send
        // 通信文件描述符，打开文件描述符，fd对应的文件偏移量一般为空，
        // 单独单文件出现发送不全，offset会自动修改当前读取位置
        int ret = (int)sendfile(cfd, fd, &offset, (size_t)(size - offset));
        if (ret == -1 && errno == EAGAIN) {
            printf("not data ....");
            perror("sendfile");
        }
        count += (int)offset;
    }
    return count;
}