#include "SystemCaller.h"

// 系统调用函数，接收命令和回调函数
std::string SystemCaller::system(const std::string& command) {
    return executeCommand(command, m_callback);
}

// 执行系统命令的内部函数
std::string SystemCaller::executeCommand(const std::string& command, ResultCallback callback) {
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    
    if (!pipe) {
        callback("Error: Failed to execute command");
        return "Error: Failed to execute command";
    }

    std::string result;
    const int bufferSize = BUFFER_SIZE;
    char buffer[bufferSize];

    while (fgets(buffer, bufferSize, pipe.get())!= nullptr) {
        result += buffer;
    }

    callback(result);
    return result;
}

void SystemCaller::registerCallback(ResultCallback callback) {
    m_callback = callback;
}