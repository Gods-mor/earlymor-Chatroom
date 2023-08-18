#pragma once
#define PORT 6666
#define HOST "127.0.0.1"

#define THREAD_NUMBERS 1

const int FIXEDWIDTH = 4;
// 心跳间隔和超时时间（单位：秒）
constexpr int HEARTBEAT_INTERVAL = 5;
constexpr int HEARTBEAT_TIMEOUT = 15;
constexpr int HEARTBEAT_IDENTIFIER = -1;
constexpr int HEARTBEAT_RESPONSE = -2;
// 客户端选项请求
constexpr int LOGIN = 1;
constexpr int REG = 2;
constexpr int GET_INFO = 3;
constexpr int FRIEND_GET_LIST = 4;
constexpr int FRIEND_TYPE = 5;
constexpr int GROUP_GET_LIST = 6;
constexpr int GROUP_TYPE = 7;
constexpr int GROUP_SET_CHAT_STATUS = 8;
constexpr int GROUP_GET_LIST_LEN = 9;
// 随机事件
constexpr int GROUP_MSG = 10;
constexpr int GROUP_CHAT_NOTICE = 11;
constexpr int FRIEND_MSG = 12;
constexpr int FRIEND_NOTICE = 13;
// 客户端好友功能请求
constexpr int FRIEND_ADD = 1;
constexpr int FRIEND_DELETE = 2;
constexpr int FRIEND_REQUIRY = 3;
constexpr int FRIEND_CHAT = 4;
constexpr int FRIEND_BLOCK = 5;
constexpr int FRIEND_CHAT_REQUIRY = 6;
// 客户端群组功能请求
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
constexpr int NOT_FRIEND = 0;
constexpr int SUCCESS_DELETE_FRIEND = 1;
constexpr int SUCCESS_REQUIRY_FRIEND = 1;
constexpr int SUCCESS_CHAT_FRIEND = 1;
constexpr int SUCCESS_SEND_MSG = 1;
constexpr int GET_FRIEND_HISTORY = 2;
constexpr int GET_GROUP_HISTORY = 2;
constexpr int FAIL_SEND_MSG = 0;
constexpr int ALREADY_TO_FILE = 3;
constexpr int SUCCESS_RECV_FILE = 4;
constexpr int NO_FILE = 5;
constexpr int FILE_LIST = 6;
constexpr int ACCESS_FILE_FAIL = 7;
constexpr int GET_FILE_SIZE = 8;
constexpr int ACCESS_FILE_SUCCESS = 9;
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
// 踢人
constexpr int SUCCESS_KICK = 1;
constexpr int FAIL_TO_KICK = 0;
// 管理员功能回应
// 退出群聊
constexpr int FAIL_TO_EXIT = 0;
constexpr int SUCCESS_EXIT = 1;
// 查看群成员
constexpr int FAIL_TO_CHECK = 0;
constexpr int SUCCESS_CHECK = 1;
// 成员功能回应
