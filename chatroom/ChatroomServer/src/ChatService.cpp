#include "ChatService.h"

ChatService::ChatService() {
    
}
ChatService::~ChatService() {}
bool ChatService::parseClientRequest(Buffer *readbuf, Buffer *writebuf,
                                     int socket) {return false;}