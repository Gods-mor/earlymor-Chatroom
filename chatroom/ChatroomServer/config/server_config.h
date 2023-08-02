#pragma once
#define MAX_CONNECTIONS 8192
#define PORT 6666
#define HOST "127.0.0.1"
#define THREAD_NUMBERS 4
constexpr int LOGIN_MSG_TYPE = 1;
constexpr int REG_MSG_TYPE = 2;

constexpr int ONE_CHAT_MSG = 1;
constexpr int GROUP_CHAT_MSG = 2;
constexpr int LOGIN_MSG_ACK = 3;
constexpr int REG_MSG_ACK = 4;

constexpr int REG_SUCCESS = 1;
constexpr int REG_FAIL = 2;
constexpr int REG_REPEAT = 3;