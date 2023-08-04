#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <vector>
#include "../config/client_config.h"
#include "TcpClient.h"

using namespace std;
using json = nlohmann::json;


int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "command invalid! example: ./ChatClient 127.0.0.1 6666" << endl;
        exit(-1);
    }
    
    TcpClient* client = new TcpClient();

    client->connectServer(argv[1],argv[2]);

       return 0;
}
