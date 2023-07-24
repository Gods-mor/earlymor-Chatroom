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
   Server server(PORT,MAX_CONNECTIONS);
   while(true){
        server.accept();
   }
}