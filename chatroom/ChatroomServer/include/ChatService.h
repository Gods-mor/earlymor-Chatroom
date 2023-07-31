#pragma once
#include "Buffer.h"

class ChatService{
    public:
    ChatService();
    ~ChatService();
    bool parseClientRequest(Buffer* readbuf,Buffer*writebuf,int socket);
    private:

};