#include "config.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Config Config::LoadFromFile(const std::string& file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open config file: " << file_path << std::endl;
        exit(1);
    }
    json j;
    ifs >> j;
    Config config;
    config.listen_port_ = j["listen_port"];
    config.redis_host_ = j["redis_host"];
    config.redis_port_ = j["redis_port"];
    return config;
}

int Config::listen_port() const {
    return listen_port_;
}

std::string Config::redis_host() const {
    return redis_host_;
}

int Config::redis_port() const {
    return redis_port_;
}