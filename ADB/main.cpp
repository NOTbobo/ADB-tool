#include "ADB.h"
#include <iostream>

int main() {
    ADB adb;
    
    adb.registerCallback([](const std::string& result) {
        std::cout << "Callback: " << result << std::endl;
    });

    adb.connect("192.168.5.33:5555");

    if (adb.status()) {
        std::cout << "Connected to " << adb.ip() << ":" << adb.port() << std::endl;
    } else {
        std::cout << "Failed to connect to device" << std::endl;
    }

    adb.disconnect();

    return 0;
}