#include "FriendService.h"
#include <dirent.h>
#include <fcntl.h>
#include <sw/redis++/redis++.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fstream>
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
        m_userFriends.clear();
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
    responseJson["type"] = FRIEND_GET_LIST;
}

void FriendService::handleFriend(json requestDataJson, json& responseJson) {
    cout << "requestType == FRIEND_TYPE" << endl;
    responseJson["type"] = FRIEND_TYPE;
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
    responseJson["type"] = FRIEND_TYPE;
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
        responseJson["type"] = FRIEND_TYPE;
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
        if (data == ":h") {
            std::vector<std::string> msg;
            m_redis->lrange(key, 0, -1, std::back_inserter(msg));
            responseJson["msg"] = msg;
            responseJson["status"] = GET_FRIEND_HISTORY;
            return;
        }
        if (data == ":f") {
            // 接受发文件的请求
            string filename = recvFile(requestDataJson, responseJson, key);
            announce(requestDataJson, responseJson, filename);
            infoRestore(key, filename);
            return;
        }
        if (data == ":r") {
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
                usleep(5);
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
        }
        announce(requestDataJson, responseJson);
        // 存储在数据库中
        infoRestore(key, requestDataJson);
    } catch (const exception& e) {
        cout << "handleFriendChat parse json error :" << e.what() << endl;
    }
}
string FriendService::recvFile(json requestDataJson,
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
    // 关闭文件
    received_file.close();
    return filename;
}
int FriendService::sendFile(int cfd, int fd, off_t offset, int size) {
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
void FriendService::announce(json requestDataJson, json& responseJson) {
    string receiver = requestDataJson["account"];
    string data = requestDataJson["data"];
    std::time_t timestamp = std::time(nullptr);
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
                connection->forwardMessageToUser(forwardMsg);
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
                cout << "chatstatus != m_account error:" << e.what() << endl;
            }
            json sendToFriend;
            sendToFriend["type"] = FRIEND_NOTICE;
            sendToFriend["account"] = m_account;
            sendToFriend["username"] = m_username;
            string forwardMsg = sendToFriend.dump();
            connection->forwardMessageToUser(forwardMsg);
        }
    }
}
void FriendService::announce(json requestDataJson,
                             json& responseJson,
                             string filename) {
    string receiver = requestDataJson["account"];
    string data =  "->" + m_account + "发送了文件" + filename+"<-";
    std::time_t timestamp = std::time(nullptr);
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
                connection->forwardMessageToUser(forwardMsg);
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
                cout << "chatstatus != m_account error:" << e.what() << endl;
            }
            json sendToFriend;
            sendToFriend["type"] = FRIEND_NOTICE;
            sendToFriend["account"] = m_account;
            sendToFriend["username"] = m_username;
            string forwardMsg = sendToFriend.dump();
            connection->forwardMessageToUser(forwardMsg);
        }
    }
}

void FriendService::infoRestore(string key,string filename){
    std::time_t timestamp = std::time(nullptr);
    json chatinfo;
    string data = "->" + m_account + "发送了文件" + filename+"<-";
    chatinfo["account"] = m_account;  // 发送者是自己
    chatinfo["timestamp"] = timestamp;
    chatinfo["data"] = data;
    string chatmsg = chatinfo.dump();
    m_redis->rpush(key, chatmsg);
}
void FriendService::infoRestore(string key,json requestDataJson){
    json chatinfo;
    std::time_t timestamp = std::time(nullptr);
    string data = requestDataJson["data"];
    chatinfo["account"] = m_account;  // 发送者是自己
    chatinfo["timestamp"] = timestamp;
    chatinfo["data"] = data;
    string chatmsg = chatinfo.dump();
    m_redis->rpush(key, chatmsg);
}