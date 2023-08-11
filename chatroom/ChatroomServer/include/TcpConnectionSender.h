#pragma once
#pragma once
#include "TcpConnection.h"  // 包含TcpConnection的头文件

class TcpConnectionSender {
public:
    TcpConnectionSender(TcpConnection* connection) : m_connection(connection) {}

    void forwardMessage(const std::string& message) {
        if (m_connection) {
            // 在这里调用 TcpConnection 的方法来发送消息
            // 例如：m_connection->send(message);
        } else {
            // 处理连接不存在的情况
        }
    }

private:
    TcpConnection* m_connection;  // 使用 TcpConnection 对象来发送消息
};
