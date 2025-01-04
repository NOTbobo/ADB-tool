cmake_minimum_required(VERSION 3.10)
project(ADBTest)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(ADBTest main.cpp ADB.cpp NetworkTool.cpp SystemCaller.cpp)