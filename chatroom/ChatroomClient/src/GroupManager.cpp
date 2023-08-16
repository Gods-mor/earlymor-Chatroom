#include "GroupManager.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "../config/client_config.h"
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
        cout << "请输入：";
        int choice;
        cin >> choice;
        cin.get();
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
    cout << "请输入要添加的群组号码：";
    cin >> id;
    cin.get();
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
    string permisson = m_tcpclient->getPermisson();
    if (permisson == "owner") {
        m_groupid = id;
        handleOwner();
    } else if (permisson == "administrator") {
        m_groupid = id;
        handleAdmin();
    } else if (permisson == "member") {
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
        cout << "请输入：";
        cin >> choice;
        cin.get();
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
        cout << "请输入：";
        cin >> choice;
        cin.get();
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
        cout << "请输入：";
        cin >> choice;
        cin.get();
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
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_OWNER;
        js["entertype"] = OWNER_CHAT;
        js["permisson"] = "owner";
        string data;
        std::time_t timestamp = std::time(nullptr);
        std::tm timeinfo;
        localtime_r(&timestamp, &timeinfo);
        std::stringstream ss;
        ss << std::put_time(&timeinfo, "%m-%d %H:%M");
        std::string formattedTime = ss.str();
        getline(cin, data);
        if (data != ":q" && data != ":h") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << YELLOW_COLOR << "[群主]"
                 << "我 " << RESET_COLOR << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;
        }
        js["data"] = data;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "data send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
        if (data == ":q") {
            break;
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
        cout << "你想要处理哪个事件(-1表示退出):" << endl;
        int number;
        cin >> number;
        cin.get();
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
            cout << "1、接受 2、拒绝" << endl;
            int choice;
            cin >> choice;
            cin.get();
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
    cout << "1、确认解散 2、返回" << endl;
    int choice;
    cin >> choice;
    cin.get();
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
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_ADMINISTRATOR;
        js["entertype"] = ADMIN_CHAT;
        js["permisson"] = "administrator";
        string data;
        std::time_t timestamp = std::time(nullptr);
        std::tm timeinfo;
        localtime_r(&timestamp, &timeinfo);
        std::stringstream ss;
        ss << std::put_time(&timeinfo, "%m-%d %H:%M");
        std::string formattedTime = ss.str();
        getline(cin, data);
        if (data != ":q" && data != ":h") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << GREEN_COLOR << "[管理员]"
                 << "我 " << RESET_COLOR << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;
        }
        js["data"] = data;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "data send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
        if (data == ":q") {
            break;
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
        cout << "你想要处理哪个事件(-1表示退出):" << endl;
        int number;
        cin >> number;
        cin.get();
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
            cout << "1、接受 2、拒绝" << endl;
            int choice;
            cin >> choice;
            cin.get();
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
    cout << "1、确认退出 2、返回" << endl;
    int choice;
    cin >> choice;
    cin.get();
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
        js["type"] = GROUP_TYPE;
        js["grouptype"] = GROUP_MEMBER;
        js["entertype"] = MEMBER_CHAT;
        js["permisson"] = "member";
        string data;
        std::time_t timestamp = std::time(nullptr);
        std::tm timeinfo;
        localtime_r(&timestamp, &timeinfo);
        std::stringstream ss;
        ss << std::put_time(&timeinfo, "%m-%d %H:%M");
        std::string formattedTime = ss.str();
        getline(cin, data);
        if (data != ":q" && data != ":h") {
            cout << "\033[A"
                 << "\33[2K\r";
            cout << "[成员]"
                 << "我 " << formattedTime << ":" << endl;
            cout << "「" << data << "」" << endl;
        }

        js["data"] = data;
        TcpClient::addDataLen(js);
        string request = js.dump();
        int len = send(m_fd, request.c_str(), strlen(request.c_str()) + 1, 0);
        if (0 == len || -1 == len) {
            cerr << "data send error:" << request << endl;
        }
        sem_wait(&m_rwsem);
        if (data == ":q") {
            break;
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
    cout << "1、确认退出 2、返回" << endl;
    int choice;
    cin >> choice;
    cin.get();
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