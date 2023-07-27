#include <arpa/inet.h>
#include <json/json.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "../config/server_config.h"
#include "server.h"

using namespace std;

int main() {
   // 读取配置文件
  Config config = Config::LoadFromFile("config.json");

  // 初始化 Redis 连接
  Redis redis(config.redis_host(), config.redis_port());

  // 初始化服务器
  Server server(config, redis);

  // 启动服务器
  server.Run();

  return 0;

}