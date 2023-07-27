#pragma once

#include <string>
#include <vector>
#include "hiredis/hiredis.h"
#include <iostream>

class Redis {
 public:
  Redis(const std::string& host, int port);
  ~Redis();

  // 获取指定键的值
  std::string Get(const std::string& key);

  // 设置指定键的值
  void Set(const std::string& key, const std::string& value);

  // 获取指定键的列表
  std::vector<std::string> GetList(const std::string& key);

  // 向指定键的列表中添加元素
  void AppendToList(const std::string& key, const std::string& value);

  // 从指定键的列表中删除元素
  void RemoveFromList(const std::string& key, const std::string& value);

  // 判断指定键是否存在
  bool Exists(const std::string& key);

 private:
  redisContext* context_;
};