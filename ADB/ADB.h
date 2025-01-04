#ifndef ADB_H
#define ADB_H

#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <cstdlib>
#include <functional>
#include "NetworkTool.h"
#include "SystemCaller.h"

// 开启ADB日志打印
#define ADB_DEBUG

class ADB {
public:
    ADB();
    
    ~ADB();

    // 定义回调函数类型
    using ResultCallback = std::function<void(const std::string&)>;

    // 连接到指定的 IP 地址，IP 地址可包含端口也可不包含
    void connect(const std::string& ip);

    // 断开连接
    void disconnect();

    // 切换到另一个 IP 地址进行连接
    void switchConnection(const std::string& ip);

    // 检测当前 ADB 连接状态
    bool status();

    // 返回当前连接的端口，如果没有连接返回 0
    int port() const;

    // 返回当前连接的 IP 地址，如果没有连接返回空字符串
    std::string ip() const;

    void registerCallback(ResultCallback callback);

private:
    std::string currentIP;
    int currentPort;
    NetworkTool networkTool;
    ResultCallback m_callback {[](const std::string&){}};
    SystemCaller m_systemCaller;
};


#endif // ADB_H