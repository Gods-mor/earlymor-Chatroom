#pragma once
#define CLIENT_BUFSIZE 1024
//菜单选项
constexpr int LOGIN = 1;
constexpr int REGISTER = 2;
constexpr int QUIT = 3;

//发送信息到服务器
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;
// 接受消息
constexpr int ONE_CHAT_MSG = 1;
constexpr int GROUP_CHAT_MSG = 2;
constexpr int LOGIN_MSG_ACK = 3;
constexpr int REG_MSG_ACK = 4;