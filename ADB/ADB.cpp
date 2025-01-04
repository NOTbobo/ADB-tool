#include "ADB.h"

ADB::ADB() : currentIP(""), currentPort(0) {}

ADB::~ADB() {}

void ADB::connect(const std::string& ip) {
    if (!networkTool.isCurrentIPValid(ip)) {
        #ifdef ADB_DEBUG
        std::cout << "current IP is not valid!" << std::endl;
        #endif

        m_callback("current IP is not valid!");
        return;
    }
    
    currentIP = networkTool.parseIP(ip);
    currentPort = networkTool.parsePort(ip);

    if (status()) {
        #ifdef ADB_DEBUG
        std::cout << "already connected to " + ip << std::endl;
        #endif
        m_callback("already connected to " + ip);
        return;
    }

    std::string command = "adb connect " + ip;
    std::string result = m_systemCaller.system(command);
    
    if (result.find("connected to") == std::string::npos) {
        #ifdef ADB_DEBUG
        std::cout << "failed to connect to " + ip << std::endl;
        #endif
        m_callback("failed to connect to " + ip);
    } else {
        #ifdef ADB_DEBUG
        std::cout << "already connected to " + ip << std::endl;
        #endif
        m_callback("already connected to " + ip);  
    }
}

void ADB::disconnect() {
    if (status()) {
        auto ip = currentIP + ":" + std::to_string(currentPort);
        auto command = "adb disconnect " + ip;
        std::string result = m_systemCaller.system(command);

        if (result.find("disconnected") == std::string::npos) {
            #ifdef ADB_DEBUG
            std::cout << "failed to disconnect from " + ip << std::endl;
            #endif
            m_callback("failed to disconnect from " + ip);
        } else {
            #ifdef ADB_DEBUG
            std::cout << "successfully disconnect from " + ip << std::endl;
            #endif
            m_callback("successfully disconnect from " + ip);  
        }

        currentIP = "";
        currentPort = 0;
    }
}

void ADB::switchConnection(const std::string& ip) {
    disconnect();
    connect(ip);
}

bool ADB::status() {
    auto ip = currentIP + ":" + std::to_string(currentPort);
    auto command = "adb devices";
    std::string result = m_systemCaller.system(command);

    if (result.find(ip) == std::string::npos) {
        return false;
    } else {
        return networkTool.checkIPConnection(currentIP, currentPort);
    }
}

int ADB::port() const {
    return currentPort;
}

std::string ADB::ip() const {
    return currentIP;
}

void ADB::registerCallback(ResultCallback callback) {
    m_callback = callback;
}