#include "client_config.h"
#include "json.hpp"
#include <arpa/inet.h>
#include <atomic>
#include <iostream>
#include <semaphore.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using json = nlohmann::json;

sem_t rwsem;                        // 用于读写线程间的通信
atomic_bool is_LoginSuccess{false}; // 原子类型，不需要加锁，用于记录登录状态

// 登录菜单
void welcomeMenu();
// 子线程用于读数据
void readTaskHandler(int cfd);
// 主菜单
void mainMenu();
// 处理登录的响应逻辑
void doLoginResponse(json &responsejs);
// 处理注册的响应逻辑
void doRegResponse(json &responsejs);

int main(int argc, char **argv) {
  if (argc < 3) {
    cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << endl;
    exit(-1);
  }
  // 解析通过命令行参数传递的ip和port
  char *ip = argv[1];
  uint16_t port = atoi(argv[2]);
  // 创建client端的socket
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clientfd) {
    cerr << "socket create error" << endl;
    exit(-1);
  }

  // 填写client需要连接的server信息ip+port
  sockaddr_in server;
  memset(&server, 0, sizeof(sockaddr_in));

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = inet_addr(ip);

  // client和server进行连接
  if (-1 == connect(clientfd, (sockaddr *)&server, sizeof(sockaddr_in))) {
    cerr << "connect server error" << endl;
    close(clientfd);
    exit(-1);
  }
  // 初始化信号量
  sem_init(&rwsem, 0, 0);

  // 子线程读数据
  // 传入cfd参数用于读数据
  // 设置线程分离
  thread readTask(readTaskHandler, clientfd);
  readTask.detach();

  //
  for (;;) {
    welcomeMenu();
    int choice = 0;
    cin >> choice;
    cin.get();
    switch (choice) {
    case LOGIN: {
      char account[12] = {0}; // 11位账号id
      char pwd[50] = {0};
      cout << "account:";
      cin >> account;
      cin.get(); // 读掉缓冲区残留的回车
      cout << "userpassword:";
      cin.getline(pwd, 50);

      json js;
      js["msgid"] = LOGIN_TYPE;
      js["account"] = account;
      js["password"] = pwd;
      string request = js.dump(); // 序列化

      int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
      if (len == -1) {
        cerr << "send login msg error:" << request << endl;
      }
      sem_wait(&rwsem); // 等待信号量，由子线程处理完登录的响应消息后，通知这里
      if (is_LoginSuccess) {
        // 进入主菜单
        mainMenu();
      }

    } break;
    case REGISTER: {
      char name[50] = {0};
      char pwd[50] = {0};
      char account[12] = {0};
      cout << "username(50字符以内):";
      cin.getline(name, 50);
      cout << "account(11位以内):";
      cin.getline(account, 12);
      cout << "userpassword(50字符以内):";
      cin.getline(pwd, 50);

      json js;
      js["msgid"] = REG_TYPE;
      js["name"] = name;
      js["account"] = account;
      string request = js.dump(); // 转为字符串

      int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
      if (len == -1) {
        cerr << "send reg msg error:" << request << endl;
      }

      sem_wait(&rwsem); // 等待信号量，子线程处理完注册消息会通知

    } break;
    case QUIT: {
      close(clientfd);
      sem_destroy(&rwsem);
      exit(0);
    }
    default:
      cerr << "invalid input!" << endl;
      break;
    }
  }
  return 0;
}
void welcomeMenu() {
  cout << "========================" << endl;
  cout << "1. login" << endl;
  cout << "2. register" << endl;
  cout << "3. quit" << endl;
  cout << "========================" << endl;
  cout << "choice:";
}
void readTaskHandler(int cfd) {
  // 死循环接收消息
  for (;;) {
    char buffer[CLIENT_BUFSIZE] = {0}; // 默认1024字符的缓冲区，可能需要扩容
    int len = recv(cfd, buffer, CLIENT_BUFSIZE, 0);
    if (-1 == len || 0 == len) {
      close(cfd);
      exit(-1);
    }
    // 接收ChatServer转发的数据，反序列化生成json数据对象
    json js = json::parse(buffer);
    int msgtype = js["msgid"].get<int>();
    // msgtype分为四类：私聊，群聊，登录，注册。
    if (ONE_CHAT_MSG == msgtype) { // 私聊
      cout << js["time"].get<string>() << " [" << js["id"] << "]"
           << js["name"].get<string>() << " said: " << js["msg"].get<string>()
           << endl;
      continue;
    }

    if (GROUP_CHAT_MSG == msgtype) { // 群聊
      cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>()
           << " [" << js["id"] << "]" << js["name"].get<string>()
           << " said: " << js["msg"].get<string>() << endl;
      continue;
    }

    if (LOGIN_MSG_ACK == msgtype) { // 登录
      doLoginResponse(js);          // 处理登录响应的业务逻辑
      sem_post(&rwsem); // 通知主线程，登录结果处理完成
      continue;
    }

    if (REG_MSG_ACK == msgtype) { // 注册
      doRegResponse(js);
      sem_post(&rwsem); // 通知主线程，注册结果处理完成
      continue;
    }
  }
}
void doLoginResponse(json &responsejs) {
  if (0 != responsejs["errno"].get<int>()) // 登录失败
  {
    cerr << responsejs["errmsg"] << endl;
    is_LoginSuccess = false;
  } else // 登录成功
  {
    cout << "登录成功" << endl;
    // 显示用户信息

    // 加载好友列表

    // 加载群组信息

    // ...
  }
}
void doRegResponse(json &responsejs) {
  if (0 != responsejs["errno"].get<int>()) // 注册失败
  {
    cerr << "user account is already exist, register error!" << endl;
  } else // 注册成功
  {
    cout << "user account register success, user account is "
         << responsejs["account"] << ", do not forget it!" << endl;
  }
}
void mainMenu() {
  cout << "this is your information!" << endl;
  sleep(1);
  exit(0);
}