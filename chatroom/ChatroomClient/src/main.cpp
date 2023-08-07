#include <iostream>
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
