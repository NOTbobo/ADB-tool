#ifndef SYSTEMCALLER_H
#define SYSTEMCALLER_H

#include <functional>
#include <string>
#include <memory>
#include <iostream>
#include <cstdio>

class SystemCaller {
public:
    // 定义回调函数类型，接收系统调用的结果作为参数
    using ResultCallback = std::function<void(const std::string&)>;

    // 构造函数
    SystemCaller() = default;

    // 析构函数
    ~SystemCaller() = default;

    // 系统调用函数，接收命令和回调函数
    std::string system(const std::string& command);

    void registerCallback(ResultCallback callback);

private:
    // 执行系统命令的内部函数
    std::string executeCommand(const std::string& command, ResultCallback callback);
    ResultCallback m_callback {[](const std::string&){}};
    int BUFFER_SIZE {1024 * 1024};
};

#endif // SYSTEMCALLER_H