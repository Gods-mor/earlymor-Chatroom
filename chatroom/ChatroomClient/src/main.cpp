#include "../config/client_config.h"
#include <arpa/inet.h>
#include <atomic>
#include <iostream>
#include <nlohmann/json.hpp>
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
void handleOneChatMessage(const json &message);
void handleGroupChatMessage(const json &message);
void handleLoginResponse(const json &message);
void handleRegisterResponse(const json &message);
// 主菜单
void mainMenu();

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
      js["type"] = LOGIN_MSG_TYPE;
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
        std::cout << "Input exceeded the maximum allowed length. Truncating..."
                  << std::endl;
        account = account.substr(0, 11); // Truncate the input to 10 characters
      }
      cout << "username(20字符以内):";
      cin >> name;
      if (name.length() > 20) {
        std::cout << "Input exceeded the maximum allowed length. Truncating..."
                  << std::endl;
        account = account.substr(0, 20); // Truncate the input to 10 characters
      }
      cout << "userpassword(20字符以内):";
      cin >> pwd;
      if (pwd.length() > 20) {
        std::cout << "Input exceeded the maximum allowed length. Truncating..."
                  << std::endl;
        account = account.substr(0, 20); // Truncate the input to 10 characters
      }
      json js;
      js["type"] = REG_MSG_TYPE;
      js["username"] = name;
      js["account"] = account;
      js["password"] = pwd;

      string request = js.dump(); // 转为字符串

      int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
      if (len == -1) {
        cerr << "send reg msg error:" << request << endl;
      }

      sem_wait(&rwsem); // 等待信号量，子线程处理完注册消息会通知

      cout << "pthread work successfully" << endl;
      break;
    }
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
  cout << "pthread start" << endl;
  for (;;) {
    char buffer[CLIENT_BUFSIZE] = {0}; // 默认1024字符的缓冲区，可能需要扩容
    cout << "recv:" << endl;
    int len = recv(cfd, buffer, CLIENT_BUFSIZE, 0);
    if (-1 == len || 0 == len) {
      cout << "error close cfd " << endl;
      close(cfd);
      sem_post(&rwsem);
      exit(-1);
    }
    // 接收ChatServer转发的数据，反序列化生成json数据对象
    try {
      json js = json::parse(buffer);
      int type = js["type"].get<int>();
      cout << "get type:" << type << endl;
      switch (type) {
      case ONE_CHAT_MSG:
        handleOneChatMessage(js);
        break;
      case GROUP_CHAT_MSG:
        handleGroupChatMessage(js);
        break;
      case LOGIN_MSG_ACK:
        handleLoginResponse(js);
        sem_post(
            &rwsem); // Notify the main thread that login response is handled
        break;
      case REG_MSG_ACK:
        handleRegisterResponse(js);
        sem_post(
            &rwsem); // Notify the main thread that register response is handled
        break;
      default:
        cerr << "Invalid message type received: " << type << endl;
        break;
      }
    } catch (const json::exception &e) {
      cerr << "Error: Failed to parse JSON data: " << e.what() << endl;
    }
  }
}

void mainMenu() {
  cout << "this is your information!" << endl;
  sleep(1);
  exit(0);
}
void handleOneChatMessage(const json &message) {
  cout << message["time"].get<string>() << " [" << message["id"] << "] "
       << message["name"].get<string>()
       << " said: " << message["msg"].get<string>() << endl;
}

void handleGroupChatMessage(const json &message) {
  cout << "Group message [" << message["groupid"]
       << "]: " << message["time"].get<string>() << " [" << message["id"]
       << "] " << message["name"].get<string>()
       << " said: " << message["msg"].get<string>() << endl;
}

// 处理登录回应
void handleLoginResponse(const json &message) {
  if (0 != message["errno"].get<int>()) {
    cerr << message["errmsg"] << endl;
    is_LoginSuccess = false;
  } else {
    cout << "Login successful" << endl;
    // TODO: Handle user information, friend list, group information, etc.
  }
}

void handleRegisterResponse(const json &message) {
  int err = message["errno"].get<int>();
  cout << "err:" << err << endl;
  if (0 == err) {
    cout << "User account register success, user account is "
         << message["account"] << ", do not forget it!" << endl;

  } else if (1 == err) {
    cout << "made mistakes! register fail!" << endl;
  } else {
    cerr << "User account is already exist, register error!" << endl;
  }
}