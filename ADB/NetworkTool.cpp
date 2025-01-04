#include "NetworkTool.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <memory>
#include <unistd.h>
#include <sys/select.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

NetworkTool::NetworkTool() {
#ifdef USE_CURL
    // 初始化 CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::cout << "libcurl initialized" << std::endl;
#endif
}

NetworkTool::~NetworkTool() {
#ifdef USE_CURL
    // 清理 CURL
    curl_global_cleanup();
#endif
}

bool NetworkTool::checkInternetConnection(const std::string& url) {
#ifdef USE_CURL
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // 不下载内容
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 设置超时时间为10秒

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);

        if(http_code == 200) {
            return true;
        }
    }
#endif
    return false;
}

bool NetworkTool::checkIPConnection(const std::string& ip, int port, int timeout) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
        return false;
    }

    // 设置socket为非阻塞模式
    int flags = fcntl(sock, F_GETFL, 0);
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set non-blocking mode: " << strerror(errno) << std::endl;
        close(sock);
        return false;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock);
        return false;
    }

    // 尝试连接
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if (errno != EINPROGRESS) {
            std::cerr << "Connection attempt failed: " << strerror(errno) << std::endl;
            close(sock);
            return false;
        }

        // 使用select等待连接完成
        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(sock, &fd);

        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;

        int selectResult = select(sock + 1, NULL, &fd, NULL, &tv);
        if (selectResult < 0) {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
            close(sock);
            return false;
        } else if (selectResult == 0) {
            std::cerr << "Connection timed out" << std::endl;
            close(sock);
            return false;
        }

        // 检查连接状态
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) != 0) {
            std::cerr << "Getsockopt error: " << strerror(errno) << std::endl;
            close(sock);
            return false;
        }
        if (error != 0) {
            std::cerr << "Connection failed: " << strerror(error) << std::endl;
            close(sock);
            return false;
        }
    }

    // std::cout << "Connection Successful" << std::endl;
    close(sock);
    return true;
}

bool NetworkTool::isCurrentIPValid(const std::string& ip) {
    size_t colonPos = ip.find(':');
    std::string ipPart = ip;
    std::string portPart;

    if (colonPos!= std::string::npos) {
        ipPart = ip.substr(0, colonPos);
        portPart = ip.substr(colonPos + 1);
    }

    if (!isValidIP(ipPart)) {
        return false;
    }

    if (!portPart.empty() &&!isValidPort(portPart)) {
        return false;
    }

    return true;
}

bool NetworkTool::isValidIP(const std::string& ip) {
    int num = 0;
    int count = 0;
    size_t i = 0;

    while (i < ip.length()) {
        if (ip[i] < '0' || ip[i] > '9') {
            return false;
        }

        num = 0;
        while (i < ip.length() && ip[i] >= '0' && ip[i] <= '9') {
            num = num * 10 + (ip[i] - '0');
            i++;
        }

        if (num > 255) {
            return false;
        }

        count++;
        if (i < ip.length() && ip[i] == '.') {
            i++;
        }
    }

    return count == 4;
}

bool NetworkTool::isValidPort(const std::string& portStr) {
    int port = 0;

    for (size_t i = 0; i < portStr.length(); i++) {
        if (portStr[i] < '0' || portStr[i] > '9') {
            return false;
        }

        port = port * 10 + (portStr[i] - '0');
    }

    return port >= 0 && port <= 65535;
}

int NetworkTool::parsePort(const std::string& ip) const {
    size_t colonPos = ip.find(':');
    if (colonPos == std::string::npos) {
        return -1; // 返回-1表示解析失败
    }
    std::string portStr = ip.substr(colonPos + 1);
    char* end;
    long portValue = std::strtol(portStr.c_str(), &end, 10);
    if (end == portStr.c_str() || *end != '\0' || portValue < 0 || portValue > 65535) {
        return -1; // 端口号无效
    }
    return static_cast<int>(portValue);
}

std::string NetworkTool::parseIP(const std::string& ip) const {
    size_t colonPos = ip.find(':');
    if (colonPos == std::string::npos) {
        return ""; // 返回空字符串表示解析失败
    }
    return ip.substr(0, colonPos);
}