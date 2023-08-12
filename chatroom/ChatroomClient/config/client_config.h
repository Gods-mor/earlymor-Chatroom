#pragma once
#define CLIENT_BUFSIZE 1024

const int FIXEDWIDTH = 4;
// 菜单选项
constexpr int LOGIN = 1;
constexpr int REGISTER = 2;
constexpr int QUIT = 3;

// 发送信息到服务器
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;
constexpr int GET_INFO_TYPE = 3;
constexpr int FRIEND_GET_LIST = 4;
constexpr int FRIEND_TYPE = 5;
constexpr int GROUP_GET_LIST = 6;
constexpr int GROUP_TYPE = 7;
// 好友功能请求
constexpr int FRIEND_ADD = 1;
constexpr int FRIEND_DELETE = 2;
constexpr int FRIEND_REQUIRY = 3;
constexpr int FRIEND_CHAT = 4;
constexpr int FRIEND_BLOCK = 5;
constexpr int FRIEND_CHAT_REQUIRY = 6;
// 接受消息
constexpr int LOGIN_MSG_ACK = 1;
constexpr int REG_MSG_ACK = 2;
constexpr int FRIEND_LIST_ACK = 3;
constexpr int FRIEND_ACK = 4;
constexpr int GET_INFO = 5;
constexpr int FRIEND_MSG = 6;
constexpr int FRIEND_NOTICE = 7;
constexpr int GROUP_LIST_ACK = 8;
// 存储用户账号信息
constexpr int GET_INFO_SUCCESS = 0;
// 在线状态
constexpr int OFFLINE = 0;
constexpr int ONLINE = 1;
// 登录状态回应
constexpr int NOT_REGISTERED = 0;
constexpr int WRONG_PASSWD = 1;
constexpr int IS_ONLINE = 2;
constexpr int PASS = 3;
constexpr int ERR = 4;

// 注册状态回应
constexpr int REGISTER_SUCCESS = 0;  // 注册成功
constexpr int REGISTER_FAILED = 1;   // 注册失败
constexpr int ACCOUNT_EXIST = 2;     // 帐号已注册
// 好友功能回应
constexpr int SUCCESS_ADD_FRIEND = 1;
constexpr int NOT_FRIEND = 0;
constexpr int SUCCESS_DELETE_FRIEND = 1;
constexpr int SUCCESS_REQUIRY_FRIEND = 1;
constexpr int SUCCESS_CHAT_FRIEND = 1;

// 群组功能请求
constexpr int GROUP_ADD = 1;
constexpr int GROUP_CREATE = 2;
// 定义颜色宏
#define RESET_COLOR "\033[0m"     // 恢复默认颜色
#define RED_COLOR "\033[31m"      // 红色
#define GREEN_COLOR "\033[32m"    // 绿色
#define YELLOW_COLOR "\033[33m"   // 黄色
#define BLUE_COLOR "\033[34m"     // 蓝色
#define MAGENTA_COLOR "\033[35m"  // 洋红色
#define CYAN_COLOR "\033[36m"     // 青色