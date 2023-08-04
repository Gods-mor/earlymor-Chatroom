#pragma once
#define PORT 6666
#define HOST "127.0.0.1"

#define THREAD_NUMBERS 1
const int FIXEDWIDTH = 4;

// 客户端选项请求
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;
constexpr int FRIEND_GET_LIST = 3;

// 服务器回应信息
constexpr int ONE_CHAT_MSG = 1;
constexpr int GROUP_CHAT_MSG = 2;
constexpr int LOGIN_MSG_ACK = 3;
constexpr int REG_MSG_ACK = 4;
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