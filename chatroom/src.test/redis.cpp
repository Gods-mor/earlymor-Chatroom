#include "redis.h"

Redis::Redis(const std::string& host, int port) {
    context_ = redisConnect(host.c_str(), port);
    if (context_ == nullptr || context_->err) {
        if (context_) {
            std::cerr << "Failed to connect to Redis: " << context_->errstr
                      << std::endl;
            redisFree(context_);
        } else {
            std::cerr << "Failed to allocate Redis context." << std::endl;
        }
        exit(1);
    }
}

Redis::~Redis() {
    redisFree(context_);
}

std::string Redis::Get(const std::string& key) {
    redisReply* reply =
        static_cast<redisReply*>(redisCommand(context_, "GET %s", key.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: GET " << key
                  << std::endl;
        exit(1);
    }
    std::string value = "";
    if (reply->type == REDIS_REPLY_STRING) {
        value = reply->str;
    }
    freeReplyObject(reply);
    return value;
}

void Redis::Set(const std::string& key, const std::string& value) {
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(context_, "SET %s %s", key.c_str(), value.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: SET " << key << " "
                  << value << std::endl;
        exit(1);
    }
    freeReplyObject(reply);
}

std::vectorstd::string Redis::GetList(const std::string& key) {
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(context_, "LRANGE %s 0 -1", key.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: LRANGE " << key
                  << std::endl;
        exit(1);
    }
    std::vectorstd::string values;
    for (size_t i = 0; i < reply->elements; i++) {
        if (reply->element[i]->type == REDIS_REPLY_STRING) {
            values.push_back(reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
    return values;
}

void Redis::AppendToList(const std::string& key, const std::string& value) {
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(context_, "RPUSH %s %s", key.c_str(), value.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: RPUSH " << key << " "
                  << value << std::endl;
        exit(1);
    }
    freeReplyObject(reply);
}

void Redis::RemoveFromList(const std::string& key, const std::string& value) {
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(context_, "LREM %s 0 %s", key.c_str(), value.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: LREM " << key << " "
                  << value << std::endl;
        exit(1);
    }
    freeReplyObject(reply);
}

bool Redis::Exists(const std::string& key) {
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(context_, "EXISTS %s", key.c_str()));
    if (reply == nullptr) {
        std::cerr << "Failed to execute Redis command: EXISTS " << key
                  << std::endl;
        exit(1);
    }
    bool exists = (reply->integer == 1);
    freeReplyObject(reply);
    return exists;
}