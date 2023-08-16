#pragma once
#define CLIENT_BUFSIZE 1024

const int FIXEDWIDTH = 4;
// 菜单选项
constexpr int LOGIN = 1;
constexpr int REGISTER = 2;
constexpr int QUIT = 3;

// 客户端发送信息到服务器
// constexpr int LOGIN = 1;
constexpr int REG = 2;
constexpr int GET_INFO = 3;
constexpr int FRIEND_GET_LIST = 4;
constexpr int FRIEND_TYPE = 5;
constexpr int GROUP_GET_LIST = 6;
constexpr int GROUP_TYPE = 7;
constexpr int GROUP_SET_CHAT_STATUS = 8;
// 随机事件
constexpr int GROUP_MSG = 9;
constexpr int GROUP_CHAT_NOTICE = 10;
constexpr int FRIEND_MSG = 11;
constexpr int FRIEND_NOTICE = 12;
// 好友功能请求
constexpr int FRIEND_ADD = 1;
constexpr int FRIEND_DELETE = 2;
constexpr int FRIEND_REQUIRY = 3;
constexpr int FRIEND_CHAT = 4;
constexpr int FRIEND_BLOCK = 5;
constexpr int FRIEND_CHAT_REQUIRY = 6;
// 群组功能请求
constexpr int GROUP_ADD = 1;
constexpr int GROUP_CREATE = 2;
constexpr int GROUP_ENTER = 3;
constexpr int GROUP_REQUIRY = 4;
constexpr int GROUP_OWNER = 5;
constexpr int GROUP_ADMINISTRATOR = 6;
constexpr int GROUP_MEMBER = 7;
constexpr int GROUP_GET_NOTICE = 8;
// 群主
constexpr int OWNER_CHAT = 1;
constexpr int OWNER_KICK = 2;
constexpr int OWNER_ADD_ADMINISTRATOR = 3;
constexpr int OWNER_REVOKE_ADMINISTRATOR = 4;
constexpr int OWNER_CHECK_MEMBER = 5;
constexpr int OWNER_CHECK_HISTORY = 6;
constexpr int OWNER_NOTICE = 7;
constexpr int OWNER_CHANGE_NAME = 8;
constexpr int OWNER_DISSOLVE = 9;
// 管理员
constexpr int ADMIN_CHAT = 1;
constexpr int ADMIN_KICK = 2;
constexpr int ADMIN_CHECK_MEMBER = 3;
constexpr int ADMIN_CHECK_HISTORY = 4;
constexpr int ADMIN_NOTICE = 5;
constexpr int ADMIN_EXIT = 6;
// 成员
constexpr int MEMBER_CHAT = 1;
constexpr int MEMBER_CHECK_MEMBER = 2;
constexpr int MEMBER_CHECK_HISTORY = 3;
constexpr int MEMBER_EXIT = 4;
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
constexpr int SUCCESS_SEND_MSG = 1;
constexpr int GET_FRIEND_HISTORY = 2;
constexpr int FAIL_SEND_MSG = 0;
// 群组功能回应
constexpr int FAIL_CREATE_GROUP = 0;
constexpr int SUCCESS_CREATE_GROUP = 1;
constexpr int FAIL_SEND_APPLICATION = 1;
constexpr int SUCCESS_SEND_APPLICATION = 2;

// 群主功能回应
// 添加管理员
constexpr int ADMIN_ALREADY_EXIST = 1;
constexpr int NOT_MEMBER = 2;
constexpr int NOT_SELF = 3;
constexpr int ADMIN_ADD_SUCCESS = 4;
// 管理员降级
constexpr int SUCCESS_REVOKE_ADMIN = 1;
// 入群申请处理
constexpr int SUCCESS_ACCEPT_MEMBER = 1;
constexpr int SUCCESS_REFUSE_MEMBER = 2;
constexpr int FAIL_DEAL_MEMBER = 3;
// set chatstaus
constexpr int SUCCESS_SET_CHATSTATUS = 1;
// 解散群聊
constexpr int DISSOLVE_FAIL = 0;
constexpr int DISSOLVE_SUCCESS = 1;

// 定义颜色宏
#define RESET_COLOR "\033[0m"     // 恢复默认颜色
#define RED_COLOR "\033[31m"      // 红色
#define GREEN_COLOR "\033[32m"    // 绿色
#define YELLOW_COLOR "\033[33m"   // 黄色
#define BLUE_COLOR "\033[34m"     // 蓝色
#define MAGENTA_COLOR "\033[35m"  // 洋红色
#define CYAN_COLOR "\033[36m"     // 青色