#include "../include/TcpConnection.h"
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
    : m_evLoop(evloop), m_redis(redis), m_onlineUsersPtr_(onlineUsersPtr_) {
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
#ifdef MSG_SEND_AUTO  // 如果被定义，
        // 断开连接
        conn->m_evLoop->AddTask(conn->m_channel, ElemType::DELETE);
#endif
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
        if (requestType == LOGIN_MSG_TYPE) {
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

        // “注册”
        else if (requestType == REG_MSG_TYPE) {
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

        // 登录初始化
        else if (requestType == GET_INFO_TYPE) {
            getInfo();
            responseJson["type"] = GET_INFO;
            responseJson["status"] = GET_INFO_SUCCESS;
        }
        //
        else if (requestType == FRIEND_GET_LIST) {
            // 计算在线好友。
            m_friendservice->getList();
            // 构建好友列表响应json

            responseJson["online_friends"] = m_friendservice->m_onlineFriends;
            responseJson["offline_friends"] = m_friendservice->m_offlineFriends;
            responseJson["type"] = FRIEND_LIST_ACK;
        } else if (requestType == FRIEND_ACK) {
            responseJson["type"] = FRIEND_ACK;
            responseJson["friendtype"] = FRIEND_ADD;
            string account = requestDataJson["account"];
            auto storedName = m_redis->hget(account, "username");

            if (!storedName) {
                responseJson["status"] = NOT_REGISTERED;
            } else {
                string name = storedName.value();
                string key = account + "_Friend";
                m_redis->hset(key, account, name);
                responseJson["status"] = SUCCESS_ADD_FRIEND;
            }
        } else if (requestType == FRIEND_DELETE) {
            responseJson["type"] = FRIEND_ACK;
            responseJson["friendtype"] = FRIEND_DELETE;
        } else if (requestType == FRIEND_REQUIRY) {
            responseJson["type"] = FRIEND_ACK;
            responseJson["friendtype"] = FRIEND_REQUIRY;
        } else if (requestType == FRIEND_CHAT) {
            responseJson["type"] = FRIEND_ACK;
            responseJson["friendtype"] = FRIEND_CHAT;
        } else if (requestType == FRIEND_BLOCK) {
            responseJson["type"] = FRIEND_ACK;
            responseJson["friendtype"] = FRIEND_BLOCK;
        } else {
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