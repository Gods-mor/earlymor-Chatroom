#pragma once

#include <unordered_map>
#include <vector>
#include "config.h"
#include "redis.h"

class Server {
 public:
  Server(const Config& config, Redis& redis);
  ~Server();

  // 启动服务器
  void Run();

 private:
  // 处理新连接请求
  void HandleNewConnection();

  // 处理客户端请求数据
  void HandleClientData(int client_fd);

  // 处理客户端退出
  void HandleClientExit(int client_fd);

  // 处理客户端登录请求
  void HandleLoginRequest(int client_fd, const std::string& username, const std::string& password);

  // 处理客户端注册请求
  void HandleRegisterRequest(int client_fd, const std::string& username, const std::string& password);

  // 处理客户端注销请求
  void HandleLogoutRequest(int client_fd);

  // 处理客户端找回密码请求
  void HandleFindPasswordRequest(int client_fd, const std::string& username, const std::string& email);

  // 处理客户端添加好友请求
  void HandleAddFriendRequest(int client_fd, const std::string& username, const std::string& friend_username);

  // 处理客户端删除好友请求
  void HandleDeleteFriendRequest(int client_fd, const std::string& username, const std::string& friend_username);

  // 处理客户端查找好友请求
  void HandleFindFriendRequest(int client_fd, const std::string& username, const std::string& friend_username);

  // 处理客户端屏蔽好友消息请求
  void HandleBlockFriendRequest(int client_fd, const std::string& username, const std::string& friend_username);

  // 处理客户端创建群组请求
  void HandleCreateGroupRequest(int client_fd, const std::string& username, const std::string& group_name);

  // 处理客户端解散群组请求
  void HandleDismissGroupRequest(int client_fd, const std::string& username, const std::string& group_name);

  // 处理客户端申请加入群组请求
  void HandleJoinGroupRequest(int client_fd, const std::string& username, const std::string& group_name);

  // 处理客户端退出群组请求
  void HandleQuitGroupRequest(int client_fd, const std::string& username, const std::string& group_name);

  // 处理客户端查找群组成员列表请求
  void HandleFindGroupMembersRequest(int client_fd, const std::string& username, const std::string& group_name);

  // 处理客户端添加群组管理员请求
  void HandleAddGroupAdminRequest(int client_fd, const std::string& username, const std::string& group_name, const std::string& admin_username);

  // 处理客户端删除群组管理员请求
  void HandleDeleteGroupAdminRequest(int client_fd, const std::string& username, const std::string& group_name, const std::string& admin_username);

  // 处理客户端批准加入群组请求
  void HandleApproveJoinGroupRequest(int client_fd, const std::string& username, const std::string& group_name, const std::string& applicant_username);

  // 处理客户端移除群组成员请求
  void HandleRemoveGroupMemberRequest(int client_fd, const std::string& username, const std::string& group_name, const std::string& member_username);

  // 处理客户端群组聊天请求
  void HandleGroupChatRequest(int client_fd, const std::string& username, const std::string& group_name, const std::string& message);

  // 处理客户端私聊请求
  void HandlePrivateChatRequest(int client_fd, const std::string& username, const std::string& friend_username, const std::string& message);

  // 发送消息给所有在线用户
  void BroadcastMessage(const std::string& message);

  // 发送消息给指定用户
  void SendMessageToUser(const std::string& username, const std::string& message);

  Config config_;
  Redis& redis_;
  int listen_fd_;
  int epoll_fd_;
  std::unordered_map<int, std::string> online_users_; // 在线用户列表
  std::unordered_map<std::string, std::string> users_; // 用户名和密码的键值对
  std::unordered_map<std::string, std::vector<std::string>> friends_; // 用户和好友列表的映射
  std::unordered_map<std::string, std::vector<std::string>> groups_; // 用户和群组列表的映射
  std::unordered_map<std::string, std::vector<std::string>> group_members_; // 群组和成员列表的映射
  std::unordered_map<std::string, std::vector<std::string>> group_admins_; // 群组和管理员列表的映射
  std::vector<std::string> blocked_friends_; // 屏蔽消息的好友列表
};