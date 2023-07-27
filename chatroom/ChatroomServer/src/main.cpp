#include <iostream>
#include <string>
#include "server.h"
#include "../config/server_config.h"
#include<unistd.h>
#include<sys/epoll.h>
#include<arpa/inet.h>

using namespace std;

int main(){
    try {
        // 创建服务器对象
        Server server(PORT,MAX_CONNECTIONS);
        server.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        // 错误处理逻辑
        return 1;
    }
    return 0;
}