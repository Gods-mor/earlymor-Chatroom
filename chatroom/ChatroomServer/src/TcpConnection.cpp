#include "../include/TcpConnection.h"
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../config/server_config.h"
#include "../include/ChatService.h"
#include "../include/log.h"

using json = nlohmann::json;

TcpConnection::TcpConnection(int fd,
                             EventLoop* evloop,
                             std::shared_ptr<sw::redis::Redis> redis,
                             std::shared_ptr<OnlineUsers> onlineUsersPtr_)
    : m_evLoop(evloop),
      m_redis(redis),
      m_onlineUsersPtr_(onlineUsersPtr_),
      m_fd(fd) {
    // 并没有创建evloop，当前的TcpConnect都是在子线程中完成的

    m_readBuf = new Buffer(10240);  // 10K
    m_writeBuf = new Buffer(10240);
    // 初始化
    // m_chatservice = new ChatService;
    m_userservice = new UserService(redis);
    m_name = "Connection-" + to_string(fd);

    m_friendservice = new FriendService(redis, onlineUsersPtr_);
    // 服务器最迫切想知道的，客户端有没有数据到达
    m_channel = new Channel(fd, FDEvent::ReadEvent, processRead, processWrite,
                            destory, this);
    Debug("Channel初始化成功....");
    // 初始化定时器
    // m_heartbeatTimerFd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    // if (m_heartbeatTimerFd == -1) {
    //     cerr << "Failed to create timerfd." << endl;
    //     // 处理错误
    // }

    // // 初始化EPOLL
    // m_epollFd = epoll_create1(0);
    // if (m_epollFd == -1) {
    //     cerr << "Failed to create epoll." << endl;
    //     // 处理错误
    // }

    // // 将定时器和连接的文件描述符添加到EPOLL中
    // struct epoll_event event;
    // event.events = EPOLLIN | EPOLLET;  // 设置边缘触发模式
    // event.data.fd = fd;
    // if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
    //     cerr << "Failed to add fd to epoll." << endl;
    //     // 处理错误
    // }

    // event.events = EPOLLIN;  // 定时器只需要读事件
    // event.data.fd = m_heartbeatTimerFd;
    // if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_heartbeatTimerFd, &event) ==
    // -1) {
    //     cerr << "Failed to add timerfd to epoll." << endl;
    //     // 处理错误
    // }

    // // 启动心跳定时器
    // startHeartbeat();
    // 把channel放到任务循环的任务队列里面
    evloop->AddTask(m_channel, ElemType::ADD);
}
// 接受数据操作
TcpConnection::~TcpConnection() {
    // 判断 读写缓存区是否还有数据没有被处理
    m_redis->hset(m_account, "status", "offline");
    if (m_readBuf && m_readBuf->readableSize() == 0 && m_writeBuf &&
        m_writeBuf->readableSize() == 0) {
        delete m_writeBuf;
        delete m_readBuf;
        delete m_chatservice;
        delete m_userservice;
        delete m_friendservice;
        delete m_groupservice;
        m_evLoop->freeChannel(m_channel);
        close(m_heartbeatTimerFd);
        close(m_epollFd);
    }

    Debug("连接断开，释放资源, connName： %s", m_name.c_str());
}

int TcpConnection::processRead(void* arg) {
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // 接受数据 最后要存储到readBuf里面
    int socket = conn->m_channel->getSocket();
    int count = conn->m_readBuf->socketRead(socket);
    // data起始地址 readPos该读的地址位置
    Debug("接收到的tcp请求数据: %s", conn->m_readBuf->data());

    if (count > 0) {
        // 接受了Client请求，解析Client请求

#ifdef MSG_SEND_AUTO
        // 添加检测写事件
        conn->m_channel->writeEventEnable(true);
        //  MODIFY修改检测读写事件
        conn->m_evLoop->AddTask(conn->m_channel, ElemType::MODIFY);
#endif
        bool flag = conn->parseClientRequest(conn->m_readBuf);
        if (!flag) {
            // 解析失败，回复一个简单的HTML
            string errMsg = "Client/1.1 400 Bad Request\r\n\r\n";
            conn->m_writeBuf->appendString(errMsg);
        }
    } else {
        // 断开连接
        conn->m_evLoop->AddTask(conn->m_channel, ElemType::DELETE);
    }
    // 断开连接 完全写入缓存区再发送不能立即关闭，还没有发送
#ifndef MSG_SEND_AUTO  // 如果没有被定义，
    // conn->m_evLoop->AddTask(conn->m_channel, ElemType::DELETE);
#endif
    return 0;
}

// 写回调函数，处理写事件，将writeBuf中的数据发送给客户端
int TcpConnection::processWrite(void* arg) {
    Debug("开始发送数据了(基于写事件发送)....");
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // 发送数据
    int count = conn->m_writeBuf->sendData(conn->m_channel->getSocket());
    if (count > 0) {
        // 判断数据是否全部被发送出去
        if (conn->m_writeBuf->readableSize() == 0) {
            // 数据发送完毕
            // 1，不再检测写事件 --修改channel中保存的事件
            conn->m_channel->writeEventEnable(false);
            // 2,
            // 修改dispatcher中检测的集合，往enentLoop反映模型认为队列节点标记为modify
            conn->m_evLoop->AddTask(conn->m_channel, ElemType::MODIFY);
            // 3，若不通信，删除这个节点
            // conn->m_evLoop->AddTask(conn->m_channel, ElemType::DELETE);
        }
    }
    return 0;
}

// tcp断开连接时断开
int TcpConnection::destory(void* arg) {
    struct TcpConnection* conn = (struct TcpConnection*)arg;
    if (conn != nullptr) {
        delete conn;
    }
    return 0;
}

// 解析客户请求即读数据-->处理业务逻辑（回应）
bool TcpConnection::parseClientRequest(Buffer* m_readBuf) {
    Debug("解析客户请求....");
    string requestData = m_readBuf->data();

    Debug("解析requestData....");
    cout << requestData << endl;
    try {
        json requestDataJson = json::parse(requestData);

        Debug("json反序列化....");
        // 从JSON数据中获取请求类型
        string lenString = requestDataJson["datalen"];
        int len = std::stoi(lenString);
        // while(len > m_readBuf->len()){
        //     // sleep(1);
        //     // 处理
        // }
        m_readBuf->readPosIncrease(len + 1);
        int requestType = requestDataJson["type"].get<int>();
        Debug("requestType:%d", requestType);
        // 根据请求类型执行不同的操作
        json responseJson;

        // "登录"
        if (requestType == HEARTBEAT_IDENTIFIER) {
            cout << "Received heartbeat from client." << endl;
            // 更新上次收到心跳的时间
            responseJson["type"] = HEARTBEAT_RESPONSE;
        } else if (requestType == LOGIN_MSG_TYPE) {
            handleLogin(requestDataJson, responseJson);
        }
        // “注册”
        else if (requestType == REG_MSG_TYPE) {
            handleRegister(requestDataJson, responseJson);
        }
        // 登录初始化
        else if (requestType == GET_INFO_TYPE) {
            handleGetInfo(requestDataJson, responseJson);
        }
        //
        else if (requestType == FRIEND_GET_LIST) {
            handleGetList(requestDataJson, responseJson);

        } else if (requestType == FRIEND_TYPE) {
            handleFriend(requestDataJson, responseJson);

        }

        else {
            // 未知的请求类型
            return false;  // 返回解析失败标志
        }
        // 将响应JSON数据添加到m_writeBuf中
        m_writeBuf->appendString(responseJson.dump());

        // 添加检测写事件
        m_channel->writeEventEnable(true);
        m_evLoop->AddTask(m_channel, ElemType::MODIFY);
    } catch (const std::exception& e) {
        cout << "Json parse error :" << e.what() << endl;
        // JSON解析错误
        return false;  // 返回解析失败标志
    }

    return true;  // 解析成功
}

// 获取登录信息
void TcpConnection::getInfo() {
    m_friendservice->getAccount(m_account);
    // 获取用户名
    string field = "username";
    auto storedUsernameOpt = m_redis->hget(m_account, field);
    m_username = storedUsernameOpt.value();
}

// 设置上线状态
void TcpConnection::setOnline() {
    m_redis->hset(m_account, "status", "online");
    m_friendservice->addOnlineNumber();  // ONlinemember++
    // 添加进入总在线用户集合
    // 将当前用户添加到总在线用户集合
    if (m_onlineUsersPtr_) {
        m_onlineUsersPtr_->addOnlineUser(m_account);
        Debug("setOnline success");
    } else {
        std::cerr << "Error: m_onlineUsersPtr_ is nullptr" << std::endl;
    }
}

void TcpConnection::startHeartbeat() {
    struct itimerspec its;
    its.it_interval.tv_sec = 5;  // 心跳间隔：5秒
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = 1;  // 第一次心跳延迟：1秒
    its.it_value.tv_nsec = 0;

    if (timerfd_settime(m_heartbeatTimerFd, 0, &its, NULL) == -1) {
        cerr << "Failed to start heartbeat timer." << endl;
        // 处理错误
    }

    // 创建线程来处理EPOLL事件
    // 在这里略去了线程的创建和管理代码
    // 可以使用线程池或其他方式来处理事件
    handleEpollEvents();
}

void TcpConnection::handleEpollEvents() {
    // 定义事件数组和缓冲区
    struct epoll_event events[2];  // 最多处理2个事件
    char buffer[8];                // 用于读取timerfd事件

    while (true) {
        int nfds = epoll_wait(m_epollFd, events, 2, -1);
        if (nfds == -1) {
            cerr << "epoll_wait error." << endl;
            // 处理错误
            continue;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == m_fd) {
                // 处理连接上的数据读取事件
                // 例如，读取客户端发送的心跳包或其他数据
                handleDataRead();
            } else if (events[i].data.fd == m_heartbeatTimerFd) {
                // 处理定时器事件
                // 读取定时器事件，以清除已触发的定时器
                read(m_heartbeatTimerFd, buffer, sizeof(buffer));
                // 发送心跳包给客户端
                sendHeartbeatPacket();
            }
        }
    }
}

void TcpConnection::sendHeartbeatPacket() {
    // 构造心跳包并发送给客户端
    // 省略具体实现...
    cout << "Sending heartbeat packet..." << endl;
}

void TcpConnection::handleDataRead() {
    // 处理连接上的数据读取事件
    // 省略具体实现...
    cout << "Handling data read..." << endl;
}

void TcpConnection::handleLogin(json requestDataJson, json& responseJson) {
    // 从JSON数据中获取输入信息（账号密码）
    std::string account = requestDataJson["account"];
    std::string password = requestDataJson["password"];
    cout << "get account:" << account << endl;
    cout << "get password:" << password << endl;
    // 处理登录请求
    int loginstatus = m_userservice->checkLogin(account, password);
    cout << "get loginstatus" << endl;

    // 登录成功则记录账号信息
    if (loginstatus == PASS) {
        m_account = account;
        setOnline();
    }

    // 构建登录响应JSON
    responseJson["type"] = LOGIN_MSG_ACK;
    responseJson["loginstatus"] = loginstatus;
}

void TcpConnection::handleRegister(json requestDataJson, json& responseJson) {
    Debug("处理注册请求");
    std::string account = requestDataJson["account"];
    std::string password = requestDataJson["password"];
    std::string username = requestDataJson["username"];
    cout << "get account:" << account << endl;
    cout << "get password:" << password << endl;
    cout << "get username:" << username << endl;
    int registerStatus =
        m_userservice->registerUser(account, password, username);
    cout << "get registerStatus:" << registerStatus << endl;
    // 构建注册响应JSON

    responseJson["type"] = REG_MSG_ACK;
    switch (registerStatus) {
        case REGISTER_SUCCESS:
            responseJson["status"] = REG_SUCCESS;
            responseJson["errno"] = 0;
            responseJson["account"] = account;
            break;
        case REGISTER_FAILED:
            responseJson["status"] = REG_FAIL;
            responseJson["errno"] = 1;
            break;
        case ACCOUNT_EXIST:
            responseJson["status"] = REG_REPEAT;
            responseJson["errno"] = 2;
            break;
    }
}

void TcpConnection::handleGetInfo(json requestDataJson, json& responseJson) {
    getInfo();
    responseJson["type"] = GET_INFO;
    responseJson["status"] = GET_INFO_SUCCESS;
}

void TcpConnection::handleGetList(json requestDataJson, json& responseJson) {
    // 计算在线好友。
    m_friendservice->getList();
    // 构建好友列表响应json

    responseJson["online_friends"] = m_friendservice->m_onlineFriends;
    responseJson["offline_friends"] = m_friendservice->m_offlineFriends;
    responseJson["type"] = FRIEND_LIST_ACK;
}

void TcpConnection::handleFriend(json requestDataJson, json& responseJson) {
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
    }
}

void TcpConnection::handleFriendAdd(json requestDataJson, json& responseJson) {
    Debug("addfriend");
    cout << "addfriend" << endl;
    responseJson["friendtype"] = FRIEND_ADD;
    string account = requestDataJson["account"];
    auto storedName = m_redis->hget(account, "username");

    if (!storedName) {
        responseJson["status"] = NOT_REGISTERED;
    } else {
        string name = storedName.value();
        string key = m_account + "_Friend";
        m_redis->hset(key, account, name);
        responseJson["status"] = SUCCESS_ADD_FRIEND;
    }
}

void TcpConnection::handleFriendDelete(json requestDataJson,
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
        responseJson["status"] = SUCCESS_DELETE_FRIEND;
    }
}

void TcpConnection::handleFriendRequiry(json requestDataJson,
                                        json& responseJson) {
    cout << "requiryfriend" << endl;
    responseJson["friendtype"] = FRIEND_DELETE;
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

void TcpConnection::handleFriendBlock(json requestDataJson,
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

void TcpConnection::handleFriendChat(json requestDataJson, json& responseJson) {
    cout << "chat with friend" << endl;
    responseJson["friendtype"] = FRIEND_CHAT;
    responseJson["type"] = FRIEND_ACK;
    string account = requestDataJson["account"];
    string key;
    if (account < m_account) {
        key = account + "+" + m_account + "_Chat";
    } else {
        key = m_account + "+" + account + "_Chat";
    }
    json chatinfo;
    string data = responseJson["data"];
    string account = responseJson["account"];
    std::time_t timestamp = std::time(nullptr);
    chatinfo["account"] = account;
    chatinfo["timestamp"] = timestamp;
    chatinfo["data"] = data;
    string chatmsg = chatinfo.dump();
    m_redis->rpush(key, chatmsg);
}

void TcpConnection::handleFriendChatWith(json requestDataJson,
                                         json& responseJson) {
    getChatHistory(requestDataJson, responseJson);
    while (true) {
        // 接收消息
    }
}
void TcpConnection::getChatHistory(json requestDataJson, json& responseJson) {}