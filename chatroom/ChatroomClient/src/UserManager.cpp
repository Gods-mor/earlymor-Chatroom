#include "UserManager.h"
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
#include "MyInput.h"
#include "TcpClient.h"
using namespace std;
using json = nlohmann::json;
// 初始化m_fd,m_rwsem
UserManager::UserManager(int fd,
                         sem_t& rwsem,
                         string& account,
                         TcpClient* tcpclient)
    : m_fd(fd), m_rwsem(rwsem), m_account(account), m_tcpclient(tcpclient) {}

UserManager::~UserManager() {
    ;
}
// 获取通知列表
void UserManager::getUserNoticeList() {
    json js;
    js["type"] = USER_GET_NOTICE;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getNotice send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void UserManager::dealNotice() {
    while (true) {
        getUserNoticeList();
        json js;
        js["type"] = USER_DEAL_NOTICE;
        js["source"] = m_account;
        int number;
        bool legal = false;
        while (!legal) {
            cout << "你想要处理哪个事件(-1表示退出):" << endl;
            legal = dataInput(number);
        }
        if (number == -1) {
            break;
        }
        if (number < 0) {
            cout << "不合法的输入" << endl;
            continue;
        }
        vector<string>& notice = m_tcpclient->m_usernotice;
        if (number >= notice.size()) {
            cout << "不合法的输入" << endl;
            continue;
        }
        string piece = notice[number];
        json noticejson = json::parse(piece);
        string type = noticejson["type"];

        if (type == "friendadd") {
            if (noticejson.contains("result")) {
                cout << "无法对其操作" << endl;
                continue;
            }
            js["number"] = number;
            int choice;
            bool legal = false;
            while (!legal) {
                cout << "1、接受 2、拒绝" << endl;
                legal = dataInput(choice);
            }
            if (choice != 1 && choice != 2) {
                cout << "不合法的输入" << endl;
                continue;
            }
            if (choice == 1) {
                js["choice"] = "accept";
            }
            if (choice == 2) {
                js["choice"] = "refuse";
            }
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "userNotice send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
        } else {
            cout << "无法对其操作" << endl;
        }
    }
}
void UserManager::showUserInfo() {}