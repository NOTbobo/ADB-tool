#ifndef NETWORK_TOOL_H
#define NETWORK_TOOL_H

#include <string>

// 如果 USE_CURL 被定义，则启用 libcurl
#ifdef USE_CURL
#include <curl/curl.h>
#endif

class NetworkTool {
public:
    NetworkTool();
    ~NetworkTool();

    bool checkInternetConnection(const std::string& url = "http://www.bing.com");
    bool checkIPConnection(const std::string& ip, int port, int timeout = 5);
    bool isCurrentIPValid(const std::string& ip);
    int parsePort(const std::string& ip) const;
    std::string parseIP(const std::string& ip) const;
    
private:
    bool isValidIP(const std::string& ip);
    bool isValidPort(const std::string& portStr);
};

#endif // NETWORK_TOOL_H