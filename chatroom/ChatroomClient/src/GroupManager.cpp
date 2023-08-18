#include "GroupManager.h"
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
#include "MyInput.h"
#include "TcpClient.h"

using namespace std;
using json = nlohmann::json;

// 初始化m_fd,m_rwsem
GroupManager::GroupManager(int fd,
                           sem_t& rwsem,
                           atomic_bool& isGroup,
                           string& account,
                           TcpClient* tcpclient)
    : m_fd(fd),
      m_rwsem(rwsem),
      is_Group(isGroup),
      m_account(account),
      m_tcpclient(tcpclient) {
    // 对unordered_map进行初始化
    unordered_map<string, string> emptyMap;
    userGroups = emptyMap;
}

GroupManager::~GroupManager() {
    ;
}

// 群组主功能菜单
void GroupManager::groupMenu() {
    cout << "Groupmenu" << endl;
    while (true) {
        // system("clear");
        // 获取群组列表
        getGroupList();
        sem_wait(&m_rwsem);
        // 打印群组菜单
        cout << "群名（群号）          未读消息数" << endl;
        for (const auto& entry : userGroups) {
            // entry.second是jsongeshi
            json info = json::parse(entry.second);
            getListLen(entry.first);
            int readmsg = info["readmsg"].get<int>();
            int count = m_tcpclient->len - readmsg;
            cout << info["groupname"] << "(" << entry.first << ")"
                 << "          " << count << endl;
        }
        showGroupFunctionMenu();
        int choice;
        bool legal = false;
        while (!legal) {
            cout << "请输入：";
            legal = dataInput(choice);
        }
        switch (choice) {
            case 1:  // 添加群组
                addGroup();
                break;
            case 2:  // 创建群组
                createGroup();
                break;
            case 3:  // 进入群组
                enterGroup();
                break;
            case 4:  // 查询群组
                requiryGroup();
                break;
            default:  // 返回
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}

void GroupManager::showGroupFunctionMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.添加群组" << endl;
    cout << "                           2.创建群组" << endl;
    cout << "                           3.进入群组" << endl;
    cout << "                           4.查询群组 " << endl;
    cout << "                           5.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::getGroupList() {
    cout << "getGroupList" << endl;
    json js;
    js["type"] = GROUP_GET_LIST;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getGroupList send error:" << request << endl;
    }
}
void GroupManager::addGroup() {
    string id;
    bool legal = false;
    while (!legal) {
        cout << "请输入要添加的群组号码：";
        legal = dataInput(id);
    }
    if (id.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        id = id.substr(0, 11);  // Truncate the input to 10 characters
    }
    string msg;
    cout << "请输入留言:";
    getline(cin, msg);
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ADD;
    js["groupid"] = id;
    js["msg"] = msg;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::createGroup() {
    string groupname;
    cout << "请输入要创建的群名称：";
    // 未来要加一些限制
    cin >> groupname;
    cin.get();
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_CREATE;
    js["owner"] = m_account;
    js["groupname"] = groupname;
    // 群组id由服务器分配
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::enterGroup() {
    m_groupid.clear();
    string id;
    cout << "请输入要进入的群组号码：";
    cin >> id;
    cin.get();
    if (id.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        id = id.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ENTER;
    js["groupid"] = id;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "enterGroup send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
    string permission = m_tcpclient->getPermission();
    if (permission == "owner") {
        m_groupid = id;
        handleOwner();
    } else if (permission == "administrator") {
        m_groupid = id;
        handleAdmin();
    } else if (permission == "member") {
        m_groupid = id;
        handleMember();
    } else {
        cout << "you are not a member yet" << endl;
    }
}
void GroupManager::ownerMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.踢人" << endl;
    cout << "                           3.添加管理员" << endl;
    cout << "                           4.撤除管理员" << endl;
    cout << "                           5.查看群成员 " << endl;
    cout << "                           6.查看历史记录" << endl;
    cout << "                           7.群通知" << endl;
    cout << "                           8.修改群名" << endl;
    cout << "                           9.解散该群" << endl;
    cout << "                           10.返回" << endl;
    cout << "               #####################################" << endl;
}

void GroupManager::administratorMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.踢人" << endl;
    cout << "                           3.查看群成员" << endl;
    cout << "                           4.查看历史记录 " << endl;
    cout << "                           5.群通知" << endl;
    cout << "                           6.退出该群" << endl;
    cout << "                           7.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::memberMenu() {
    cout << "               #####################################" << endl;
    cout << "                           1.聊天" << endl;
    cout << "                           2.查看群成员" << endl;
    cout << "                           3.查看历史记录" << endl;
    cout << "                           4.退出该群 " << endl;
    cout << "                           5.返回" << endl;
    cout << "               #####################################" << endl;
}
void GroupManager::handleOwner() {
    cout << "you are the owner" << endl;
    while (true) {
        ownerMenu();
        int choice;
        bool legal = false;
        while (!legal) {
            cout << "请输入：";
            legal = dataInput(choice);
        }
        switch (choice) {
            case 1:
                ownerChat();
                break;
            case 2:
                ownerKick();
                break;
            case 3:
                ownerAddAdministrator();
                break;
            case 4:
                ownerRevokeAdministrator();
            case 5:
                ownerCheckMember();
                break;
            case 6:
                ownerCheckHistory();
                break;
            case 7:
                ownerNotice();
                break;
            case 8:
                ownerChangeName();
                break;
            case 9:
                ownerDissolve();
                choice = 0;
                break;
            default:
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}

void GroupManager::handleAdmin() {
    cout << "you are the Administrator" << endl;
    while (true) {
        administratorMenu();
        int choice;
        bool legal = false;
        while (!legal) {
            cout << "请输入：";
            legal = dataInput(choice);
        }
        switch (choice) {
            case 1:
                adminChat();
                break;
            case 2:
                adminKick();
                break;
            case 3:
                adminCheckMember();
                break;
            case 4:
                adminCheckHistory();
                break;
            case 5:
                adminNotice();
                break;
            case 6:
                adminExit();
                choice = 0;
                break;
            default:
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}

void GroupManager::handleMember() {
    cout << "you are the Member" << endl;
    while (true) {
        memberMenu();
        int choice;
        bool legal = false;
        while (!legal) {
            cout << "请输入：";
            legal = dataInput(choice);
        }
        switch (choice) {
            case 1:
                memberChat();
                break;
            case 2:
                memberCheckMember();
                break;
            case 3:
                memberCheckHistory();
                break;
            case 4:
                memberExit();
                choice = 0;
                break;
            default:
                choice = 0;
                break;
        }
        if (choice == 0) {
            break;
        }
    }
}
void GroupManager::ownerChat() {
    setChatStatus();
    while (true) {
        json js;
        chatRequest(js, "owner");
        string data;
        chatInput(data, "owner");
        if (data == ":f") {
            chooseFile(js);
        } else if (data == ":r") {
            // 获取文件列表
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            string filename;
            cin >> filename;
            cin.get();
            js["filename"] = filename;
            TcpClient::addDataLen(js);
            request.clear();
            request = js.dump();
            len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);

        } else {
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            if (data == ":q") {
                break;
            }
        }
    }
}
// 踢出成员
void GroupManager::ownerKick() {
    cout << "请输入你想要踢出群聊的成员账号：";
    string account;
    cin >> account;
    cin.get();
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_OWNER;
    js["entertype"] = OWNER_KICK;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "ownerKick send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
// 添加管理员
void GroupManager::ownerAddAdministrator() {
    cout << "请输入你想要提升为管理员的成员账号：";
    string account;
    cin >> account;
    cin.get();
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_OWNER;
    js["entertype"] = OWNER_ADD_ADMINISTRATOR;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "addAdministrator send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
// 撤除管理员身份
void GroupManager::ownerRevokeAdministrator() {
    cout << "请输入你想要撤除管理员的成员账号：";
    string account;
    cin >> account;
    cin.get();
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_OWNER;
    js["entertype"] = OWNER_REVOKE_ADMINISTRATOR;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "ownerRevokeAdministrator send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}

void GroupManager::ownerCheckMember() {
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_OWNER;
    js["entertype"] = OWNER_CHECK_MEMBER;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "ownerKick send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::ownerCheckHistory() {}
// 处理公告
void GroupManager::ownerNotice() {
    // 展示通知列表
    while (true) {
        getNotice();
        json js;
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_OWNER;
        js["entertype"] = OWNER_NOTICE;
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
        vector<string>& notice = m_tcpclient->m_groupnotice;
        if (number >= notice.size()) {
            cout << "不合法的输入" << endl;
            continue;
        }
        string piece = notice[number];
        json noticejson = json::parse(piece);
        string type = noticejson["type"];

        if (type == "add") {
            if (noticejson.contains("dealer")) {
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
                cerr << "ownerNotice send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
        } else {
            cout << "无法对其操作" << endl;
        }
    }
}

void GroupManager::ownerChangeName() {}
void GroupManager::ownerDissolve() {
    int choice;
    bool legal = false;
    while (!legal) {
        cout << "1、确认解散 2、返回" << endl;
        legal = dataInput(choice);
    }
    if (choice == 1) {
        json js;
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_OWNER;
        js["entertype"] = OWNER_DISSOLVE;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "ownerRevokeAdministrator send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
    } else if (choice == 2) {
        return;
    } else {
        cout << "输入不合法" << endl;
    }
}

void GroupManager::adminChat() {
    setChatStatus();
    while (true) {
        json js;
        chatRequest(js, "administrator");
        string data;
        chatInput(data, "administrator");
        if (data == ":f") {
            chooseFile(js);
        } else if (data == ":r") {
            // 获取文件列表
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            string filename;
            cin >> filename;
            cin.get();
            js["filename"] = filename;
            TcpClient::addDataLen(js);
            request.clear();
            request = js.dump();
            len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);

        } else {
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            if (data == ":q") {
                break;
            }
        }
    }
}
void GroupManager::adminKick() {
    cout << "请输入你想要踢出群聊的成员账号：";
    string account;
    cin >> account;
    cin.get();
    if (account.length() > 11) {
        std::cout << "Input exceeded the maximum allowed length. "
                     "Truncating..."
                  << std::endl;
        account = account.substr(0, 11);  // Truncate the input to 10 characters
    }
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ADMINISTRATOR;
    js["entertype"] = ADMIN_KICK;
    js["account"] = account;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "adminKick send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::adminCheckMember() {
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_ADMINISTRATOR;
    js["entertype"] = ADMIN_CHECK_MEMBER;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "ownerKick send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::adminCheckHistory() {}
void GroupManager::adminNotice() {
    while (true) {
        getNotice();
        json js;
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_ADMINISTRATOR;
        js["entertype"] = ADMIN_NOTICE;
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
        vector<string>& notice = m_tcpclient->m_groupnotice;
        if (number >= notice.size()) {
            cout << "不合法的输入" << endl;
            continue;
        }
        string piece = notice[number];
        json noticejson = json::parse(piece);
        string type = noticejson["type"];

        if (type == "add") {
            if (noticejson.contains("dealer")) {
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
                cerr << "ownerNotice send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
        } else {
            cout << "无法对其操作" << endl;
        }
    }
}
void GroupManager::adminExit() {
    int choice;
    bool legal = false;
    while (!legal) {
        cout << "1、确认退出 2、返回" << endl;
        legal = dataInput(choice);
    }
    if (choice == 1) {
        json js;
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_ADMINISTRATOR;
        js["entertype"] = ADMIN_EXIT;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "ownerRevokeAdministrator send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
    } else if (choice == 2) {
        return;
    } else {
        cout << "输入不合法" << endl;
    }
}

void GroupManager::memberChat() {
    setChatStatus();
    while (true) {
        json js;
        chatRequest(js, "member");
        string data;
        chatInput(data, "member");
        if (data == ":f") {
            chooseFile(js);
        } else if (data == ":r") {
            // 获取文件列表
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            string filename;
            cin >> filename;
            cin.get();
            js["filename"] = filename;
            TcpClient::addDataLen(js);
            request.clear();
            request = js.dump();
            len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);

        } else {
            js["data"] = data;
            TcpClient::addDataLen(js);
            string request = js.dump();
            int len =
                send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (0 == len || -1 == len) {
                cerr << "data send error:" << request << endl;
            }
            sem_wait(&m_rwsem);
            if (data == ":q") {
                break;
            }
        }
    }
}
void GroupManager::memberCheckMember() {
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_MEMBER;
    js["entertype"] = MEMBER_CHECK_MEMBER;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "ownerKick send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::memberCheckHistory() {}
void GroupManager::memberExit() {
    int choice;
    bool legal = false;
    while (!legal) {
        cout << "1、确认退出 2、返回" << endl;
        legal = dataInput(choice);
    }
    if (choice == 1) {
        json js;
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_MEMBER;
        js["entertype"] = MEMBER_EXIT;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "ownerRevokeAdministrator send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
    } else if (choice == 2) {
        return;
    } else {
        cout << "输入不合法" << endl;
    }
}
void GroupManager::requiryGroup() {}
void GroupManager::getNotice() {
    json js;
    js["type"] = GROUP_TYPE;
    js["grouptype"] = GROUP_GET_NOTICE;
    js["groupid"] = m_groupid;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getNotice send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::setChatStatus() {
    json js;
    js["type"] = GROUP_SET_CHAT_STATUS;
    js["groupid"] = m_groupid;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "setChatStatus send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
void GroupManager::getListLen(const string groupid) {
    json js;
    js["type"] = GROUP_GET_LIST_LEN;
    js["groupid"] = groupid;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "getListLen send error:" << request << endl;
    }
    sem_wait(&m_rwsem);
}
int GroupManager::sendFile(int cfd, int fd, off_t offset, int size) {
    int count = 0;
    while (offset < size) {
        // 系统函数，发送文件，linux内核提供的sendfile 也能减少拷贝次数
        //  sendfile发送文件效率高，而文件目录使用send
        // 通信文件描述符，打开文件描述符，fd对应的文件偏移量一般为空，
        // 单独单文件出现发送不全，offset会自动修改当前读取位置
        int ret = (int)sendfile(cfd, fd, &offset, (size_t)(size - offset));
        if (ret == -1 && errno == EAGAIN) {
            printf("not data ....");
            perror("sendfile");
        }
        count += (int)offset;
    }
    return count;
}
void GroupManager::chatRequest(json& js, string permission) {
    js["type"] = GROUP_TYPE;
    js["permission"] = permission;
    if (permission == "owner") {
        js["grouptype"] = GROUP_OWNER;
        js["entertype"] = OWNER_CHAT;

    } else if (permission == "administrator") {
        js["grouptype"] = GROUP_ADMINISTRATOR;
        js["entertype"] = ADMIN_CHAT;

    } else if (permission == "member") {
        js["grouptype"] = GROUP_MEMBER;
        js["entertype"] = MEMBER_CHAT;
    }
}

void GroupManager::chatInput(string& data, string permission) {
    std::time_t timestamp = std::time(nullptr);
    std::tm timeinfo;
    localtime_r(&timestamp, &timeinfo);
    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%m-%d %H:%M");
    std::string formattedTime = ss.str();
    getline(cin, data);
    if (data != ":q" && data != ":h" && data != ":f" && data != ":r") {
        if (permission == "owner") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << YELLOW_COLOR << "[群主]"
                 << "我 " << RESET_COLOR << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;
        } else if (permission == "administrator") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << GREEN_COLOR << "[管理员]"
                 << "我 " << RESET_COLOR << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;

        } else if (permission == "member") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << "[成员]"
                 << "我 " << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;
        }
    }
}
void GroupManager::chooseFile(json& js) {
    string filepath;
    cout << "请输入文件的相对路径：" << endl;
    cin >> filepath;
    cin.get();
    js["filepath"] = filepath;
    js["data"] = ":f";

    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open");
        cout << "发送失败，返回聊天" << endl;
        return;
    }
    // 获取文件大小
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        close(fd);
        cout << "发送失败，返回聊天" << endl;
        return;
    }
    js["filesize"] = file_stat.st_size;
    TcpClient::addDataLen(js);
    string request = js.dump();
    int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
    if (0 == len || -1 == len) {
        cerr << "data send error:" << request << endl;
    }
    sem_wait(&m_rwsem);

    off_t offset = 0;  // 从文件开始位置开始发送
    size_t remaining_bytes = file_stat.st_size;  // 剩余要发送的字节数
    // 使用 sendfile 函数发送文件内容
    ssize_t sent_bytes = sendFile(m_fd, fd, offset, remaining_bytes);

    if (sent_bytes == -1) {
        perror("sendfile");
        close(fd);
        cout << "发送失败，返回聊天" << endl;
        return;
    }
    if (sent_bytes == file_stat.st_size) {
        cout << "发送中..." << endl;
    }
}