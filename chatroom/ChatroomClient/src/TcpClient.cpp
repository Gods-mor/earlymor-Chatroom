#include "TcpClient.h"
#include <stdlib.h>

using namespace std;
// client初始化，socket,sem
TcpClient::TcpClient() {
    // 创建client端的socket
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_fd) {
        std::cerr << "socket create error" << std::endl;
        exit(-1);
    }
    // 初始化信号量
    
    sem_init(&m_rwsem, 0, 0);
    m_friendmanager = new FriendManager(m_fd, m_rwsem);
}

// 析构回收资源
TcpClient::~TcpClient() {
    ;
}

// 连接server -> 启动client
void TcpClient::connectServer(char* arg1, char* arg2) {
    // 解析通过命令行参数传递的ip和port
    char* ip = arg1;
    uint16_t port = atoi(arg2);
    // 填写client需要连接的server信息ip+port
    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    if (-1 == connect(m_fd, (sockaddr*)&server, sizeof(sockaddr_in))) {
        std::cerr << "connect server error" << std::endl;
        close(m_fd);
        exit(-1);
    }

    this->run();
}

// 启动client,启动读子线程
void TcpClient::run() {
    // 子线程读数据
    // 传入cfd参数用于读数据
    // 设置线程分离
    m_readTask = new thread(std::bind(&TcpClient::readTaskHandler, this, m_fd));
    m_readTask->detach();
    for (;;) {
        welcomeMenu();
        int choice = 0;
        cin >> choice;
        cin.get();
        switch (choice) {
            case LOGIN: {
                std::string account;  // 11位账号id
                std::string pwd;
                cout << "不支持输入空格" << endl;
                cout << "account:";
                cin >> account;
                cout << "userpassword:";
                cin >> pwd;

                json js;
                js["type"] = LOGIN_MSG_TYPE;
                js["account"] = account;
                js["password"] = pwd;
                addDataLen(js);
                string request = js.dump();
                int len =
                    send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
                if (len == -1) {
                    cerr << "send login msg error:" << request << endl;
                }
                sem_wait(
                    &m_rwsem);  // 等待信号量，由子线程处理完登录的响应消息后，通知这里
                if (is_LoginSuccess) {
                    // 初始化个人信息
                    getInfo(account);
                    // 进入主菜单
                    mainMenu();

                    while (true) {
                        int mainMenuChoice;
                        cin >> mainMenuChoice;
                        sleep(1);
                        // system("clear");
                        switch (mainMenuChoice) {
                            case 1:
                                m_friendmanager->fiendMenu();
                                break;
                            case 2:
                                break;
                            case 3:
                                break;
                            case 4:
                                break;
                            default:
                                // 当进入default分支时，设置条件为false，退出循环
                                mainMenuChoice = 0;
                                break;
                        }
                        if (mainMenuChoice == 0) {
                            // 在default分支中设置mainMenuChoice为0，退出循环
                            break;
                        }
                    }
                }
                break;
            }
            case REGISTER: {
                std::string account;
                std::string name;
                std::string pwd;
                cout << "不支持输入空格" << endl;
                cout << "account(11位以内):";
                cin >> account;
                if (account.length() > 11) {
                    std::cout << "Input exceeded the maximum allowed length. "
                                 "Truncating..."
                              << std::endl;
                    account = account.substr(
                        0, 11);  // Truncate the input to 11 characters
                }
                cout << "username(20字符以内):";
                cin >> name;
                if (name.length() > 20) {
                    std::cout << "Input exceeded the maximum allowed length. "
                                 "Truncating..."
                              << std::endl;
                    account = account.substr(
                        0, 20);  // Truncate the input to 10 characters
                }
                cout << "userpassword(20字符以内):";
                cin >> pwd;
                if (pwd.length() > 20) {
                    std::cout << "Input exceeded the maximum allowed length. "
                                 "Truncating..."
                              << std::endl;
                    account = account.substr(
                        0, 20);  // Truncate the input to 10 characters
                }
                json js;
                js["type"] = REG_MSG_TYPE;
                js["username"] = name;
                js["account"] = account;
                js["password"] = pwd;
                addDataLen(js);
                string request = js.dump();

                int len =
                    send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
                if (len == -1) {
                    cerr << "send reg msg error:" << request << endl;
                }

                sem_wait(&m_rwsem);  // 等待信号量，子线程处理完注册消息会通知

                cout << "pthread work successfully" << endl;
                break;
            }
            case QUIT: {
                close(m_fd);
                sem_destroy(&m_rwsem);
                exit(0);
            }
            default:
                cerr << "invalid input!" << endl;
                break;
        }
    }
}

// 读任务处理器
void TcpClient::readTaskHandler(int cfd) {
    // 死循环接收消息
    cout << "pthread start" << endl;
    for (;;) {
        char buffer[CLIENT_BUFSIZE] = {
            0};  // 默认1024字符的缓冲区，可能需要扩容
        cout << "recv:" << endl;
        int len = recv(cfd, buffer, CLIENT_BUFSIZE, 0);
        if (-1 == len || 0 == len) {
            cout << "error close cfd " << endl;
            close(cfd);
            sem_post(&m_rwsem);
            exit(-1);
        }
        // 接收ChatServer转发的数据，反序列化生成json数据对象
        try {
            json js = json::parse(buffer);
            int type = js["type"].get<int>();
            cout << "get type:" << type << endl;
            js.erase("type");  // 剔除type字段，只保留数据
            switch (type) {
                case LOGIN_MSG_ACK:
                    handleLoginResponse(js);
                    sem_post(&m_rwsem);  // Notify the main thread that login
                                         // response is handled
                    break;
                case REG_MSG_ACK:
                    handleRegisterResponse(js);
                    sem_post(&m_rwsem);  // Notify the main thread that register
                                         // response is handled
                    break;
                case FRIEND_LIST_ACK:
                    handleFriendListResponse(js);
                    sem_post(&m_rwsem);
                    break;
                case FRIEND_ACK:
                    int friendtype;
                    friendtype = js["friendtype"].get<int>();
                    switch (friendtype) {
                        case FRIEND_ADD:
                            handleFriendAddResponse(js);
                            break;
                        case FRIEND_DELETE:
                            handleFriendDeleteResponse(js);
                            break;
                        case FRIEND_REQUIRY:
                            handleFriendRequiryResponse(js);
                            break;
                        case FRIEND_CHAT:
                            handleFriendChatResponse(js);
                            break;
                        case FRIEND_BLOCK:
                            handleFriendBlockResponse(js);
                            break;
                    }
                    sem_post(&m_rwsem);
                    break;
                case GET_INFO:
                    sem_post(&m_rwsem);
                    break;
                default:
                    cerr << "Invalid message type received: " << type << endl;
                    break;
            }
        } catch (const json::exception& e) {
            cerr << "Error: Failed to parse JSON data: " << e.what() << endl;
        }
    }
}

// 处理登录回应
void TcpClient::handleLoginResponse(const json& message) {
    int loginstatus = message["loginstatus"].get<int>();
    is_LoginSuccess = false;
    if (NOT_REGISTERED == loginstatus) {
        cout << "account didn't registered!" << endl;
    } else if (WRONG_PASSWD == loginstatus) {
        cout << "wrong password!" << endl;
        // TODO: Handle user information, friend list, group information, etc.
    } else if (IS_ONLINE == loginstatus) {
        cout << "account online" << endl;
        // TODO: Handle user information, friend list, group information, etc.
    } else if (PASS == loginstatus) {
        cout << "Login successful" << endl;
        is_LoginSuccess = true;
        // TODO: Handle user information, friend list, group information, etc.
    } else if (ERR == loginstatus) {
        cout << "make mistakes!" << endl;
    }
}

// 处理注册回应
void TcpClient::handleRegisterResponse(const json& message) {
    int err = message["errno"].get<int>();
    cout << "err:" << err << endl;
    if (REGISTER_SUCCESS == err) {
        cout << "User account register success, user account is "
             << message["account"] << ", do not forget it!" << endl;

    } else if (ACCOUNT_EXIST == err) {
        cerr << "User account is already exist, register error!" << endl;

    } else {
        cout << "made mistakes! register fail!" << endl;
    }
}

// 好友间聊天处理
void TcpClient::handleOneChatMessage(const json& message) {
    ;
}

// 群聊处理
void TcpClient::handleGroupChatMessage(const json& message) {
    ;
}

// 获取好友列表
void TcpClient::handleFriendListResponse(const json& message) {
    try {
        cout << "----handleFriendListResponse" << endl;
        m_friendmanager->onlineFriends = message["online_friends"];
        m_friendmanager->offlineFriends = message["offline_friends"];
    } catch (const exception& e) {
        cout << "handleFriendListResponse error :" << e.what() << endl;
    }
}

// 处理添加好友请求回应
void TcpClient::handleFriendAddResponse(const json& message) {
    int status = message["status"].get<int>();
    if (status == NOT_REGISTERED) {
        cout << "account didn't register" << endl;
    }
    if (status == SUCCESS_ADD_FRIEND) {
        cout << "add friend successfully!" << endl;
    }
}

// 处理删除好友请求回应
void TcpClient::handleFriendDeleteResponse(const json& message) {}

// 处理好友聊天请求回应
void TcpClient::handleFriendChatResponse(const json& message) {}

// 处理查询好友请求回应
void TcpClient::handleFriendRequiryResponse(const json& message) {}

// 处理拉黑好友请求回应
void TcpClient::handleFriendBlockResponse(const json& message) {}

// 向数据中加入数据长度
void TcpClient::addDataLen(json& js) {
    string prerequest = js.dump();  // 序列化
    int datalen = prerequest.length() + 13 + FIXEDWIDTH;
    std::string strNumber = std::to_string(datalen);
    std::string paddedStrNumber =
        std::string(FIXEDWIDTH - strNumber.length(), '0') + strNumber;
    js["datalen"] = paddedStrNumber;
}

// 登录菜单
void TcpClient::welcomeMenu() {
    cout << "========================" << endl;
    cout << "1. login" << endl;
    cout << "2. register" << endl;
    cout << "3. quit" << endl;
    cout << "========================" << endl;
    cout << "choice:";
}

// 主菜单
void TcpClient::mainMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.好友" << endl;
    cout << "                           2.群" << endl;
    cout << "                           3.个人信息 " << endl;
    cout << "                           4.退出" << endl;
    cout << "               #####################################" << endl;
}

// 得到账号信息
void TcpClient::getInfo(string account) {
    json js;
    js["type"] = GET_INFO_TYPE;
    addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getInfo send error:" << request << endl;
    }
}