#pragma once

#include <string>
#include <json/json.h>

class Config {
 public:
  Config() = default;
  ~Config() = default;

  // 从文件中加载配置信息
  static Config LoadFromFile(const std::string& file_path);

  // 获取监听端口
  int listen_port() const;

  // 获取 Redis 主机
  std::string redis_host() const;

  // 获取 Redis 端口
  int redis_port() const;

 private:
  int listen_port_;
  std::string redis_host_;
  int redis_port_;
};