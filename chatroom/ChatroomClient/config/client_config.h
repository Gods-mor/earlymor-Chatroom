#pragma once
#define CLIENT_BUFSIZE 1024
//菜单选项
constexpr int LOGIN = 1;
constexpr int REGISTER = 2;
constexpr int QUIT = 3;

//发送信息到服务器
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;
constexpr int ONE_CHAT_MSG = 3;
constexpr int GROUP_CHAT_MSG = 4;
constexpr int LOGIN_MSG_ACK = 5;
constexpr int REG_MSG_ACK = 6;