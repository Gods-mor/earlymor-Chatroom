#include "../include/FriendService.h"
#include <sw/redis++/redis++.h>
FriendService::FriendService(std::shared_ptr<sw::redis::Redis> redis)
    : m_redis(redis) {}
FriendService::~FriendService() {}


void FriendService::getList(){
    
}