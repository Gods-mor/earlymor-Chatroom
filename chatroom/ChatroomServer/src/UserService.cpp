#include "UserService.h"
#include <sw/redis++/redis++.h>
#include <string>
#include "../config/server_config.h"
#include "log.h"

using namespace std;

UserService::UserService(std::shared_ptr<sw::redis::Redis> redis)
    : m_redis(redis) {}

UserService::~UserService() {}

int UserService::checkLogin(string account, string password) {
    try {
        Debug("loginUser");
        string field1 = "password";
        auto storedPasswordOpt = m_redis->hget(account, field1);
        std::string storedPassword;
        Debug("getpassword");
        if (storedPasswordOpt) {                         // 账号已注册
            storedPassword = storedPasswordOpt.value();  // 获取密码
        } else {
            return NOT_REGISTERED;
        }
        if (password != storedPassword) {  // 密码不正确
            return WRONG_PASSWD;
        }
        if (isLogin(account)) {  // 已登录
            return IS_ONLINE;
        }
        return PASS;
    } catch (const std::exception& ex) {
        std::cerr << "Error while getting password from Redis: " << ex.what()
                  << std::endl;
        return ERR;
    }
}
bool UserService::isLogin(const std::string& account) {
    try {
        string field = "status";
        auto reply = m_redis->hget(account, field);
        std::string replyvalue = reply.value();
        std::string online = "online";
        return (replyvalue == online);
    } catch (const std::exception& ex) {
        std::cerr << "Error while checking login status in Redis: " << ex.what()
                  << std::endl;
        return false;
    }
}
int UserService::registerUser(string account,
                              string password,
                              string username) {
    try {
        Debug("registerUser");
        std::string storedPassword;
        auto storedPasswordOpt = m_redis->hget(account, "password");
        Debug("getpassword");
        if (storedPasswordOpt) {  // 帐号已注册
            Debug("ACCOUNT_EXIST");
            return ACCOUNT_EXIST;
        }
        std::string field1 = "username";
        std::string value1 = username;
        std::string field2 = "password";
        std::string value2 = password;
        std::string field3 = "status";
        std::string value3 = "offline";
        std::string field4 = "chatstatus";
        std::string value4 = "";
        // 将用户信息存储到 Redis 数据库中
        // 使用哈希表存储 （账号，密码，用户名，在线状态）
        m_redis->hset(account, field1, value1);
        m_redis->hset(account, field2, value2);
        m_redis->hset(account, field3, value3);
        m_redis->hset(account, field4, value4);
        // 注册成功
        Debug("REGISTER_SUCCESS");
        return REGISTER_SUCCESS;
    } catch (const sw::redis::Error& ex) {
        // 捕获异常并处理
        std::cerr << "Exception occurred: " << ex.what() << std::endl;
        // 注册失败
        Debug("REGISTER_FAILED");
        return REGISTER_FAILED;
    }
}

void UserService::getNotice(json requestDataJson, json& responseJson) {
    string key = m_account + "_Notice";
    vector<string> userNotice;
    m_redis->lrange(key, 0, -1, std::back_inserter(userNotice));
    responseJson["notice"] = userNotice;
    responseJson["type"] = USER_GET_NOTICE;
}

void UserService::dealNotice(json requestDataJson, json& responseJson) {
    string notice = m_account + "_Notice";
    responseJson["type"] = USER_DEAL_NOTICE;
    int number = requestDataJson["number"].get<int>();
    cout << "get number" << number << endl;
    string msg = m_redis->lindex(notice, number).value();
    json info;
    info = json::parse(msg);
    cout << "get info" << info << endl;
    string account = info["src"];
    cout << "get account" << account << endl;
    string choice = requestDataJson["choice"];
    cout << "get choice" << choice << endl;
    info["dealer"] = m_account;
    info["result"] = choice;
    string infostr = info.dump();
    if (choice == "accept") {
        string name = m_redis->hget(account, "username").value();
        int num = 0;
        json jsonvalue;
        string key = m_account + "_Friend";
        string key2 = account + "_Friend";
        jsonvalue["username"] = name;
        jsonvalue["unreadmsg"] = num;
        string value = jsonvalue.dump();

        json jsonvalue2;
        jsonvalue2["username"] = m_username;
        jsonvalue2["unreadmsg"] = num;
        string value2 = jsonvalue2.dump();
        m_redis->hset(key, account, value);
        m_redis->hset(key2, m_account, value2);
        m_redis->lset(notice, number, infostr);
        responseJson["status"] = SUCCESS_ACCEPT_FRIEND;
    } else if (choice == "refuse") {
        m_redis->lset(notice, number, infostr);
        responseJson["status"] = SUCCESS_REFUSE_FRIEND;
    } else {
        responseJson["status"] = FAIL_DEAL_FRIEND;
    }
}
