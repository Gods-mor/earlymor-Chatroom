#pragma once
#define PORT 6666
#define HOST "127.0.0.1"

#define THREAD_NUMBERS 1
const int FIXEDWIDTH = 4;

// 客户端选项请求
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;
constexpr int GET_INFO_TYPE = 3;
constexpr int FRIEND_GET_LIST = 4;

// 客户端好友功能请求
constexpr int FRIEND_ADD = 1;
constexpr int FRIEND_DELETE = 2;
constexpr int FRIEND_REQUIRY = 3;
constexpr int FRIEND_CHAT = 4;
constexpr int FRIEND_BLOCK = 5;
// 服务器回应信息
constexpr int LOGIN_MSG_ACK = 1;
constexpr int REG_MSG_ACK = 2;
constexpr int FRIEND_LIST_ACK = 3;
constexpr int FRIEND_ACK = 4;
constexpr int GET_INFO = 5;

//
constexpr int GET_INFO_SUCCESS = 0;
// 注册状态回应
constexpr int REG_SUCCESS = 1;
constexpr int REG_FAIL = 2;
constexpr int REG_REPEAT = 3;
// 登录状态
constexpr int OFFLINE = 0;
constexpr int ONLINE = 1;
// 登录状态回应
constexpr int NOT_REGISTERED = 0;
constexpr int WRONG_PASSWD = 1;
constexpr int IS_ONLINE = 2;
constexpr int PASS = 3;
constexpr int ERR = 4;
// 好友功能回应
constexpr int SUCCESS_ADD_FRIEND = 1;