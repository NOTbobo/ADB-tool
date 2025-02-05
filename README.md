# ADBTest

这是一个使用ADB库进行测试的示例项目。

## 简介

ADBTest 是一个用于测试 ADB 功能的项目。它提供了一个简单的命令行界面，允许用户连接、断开和切换 ADB 连接。

## 功能

- 连接到指定的 IP 地址和端口
- 断开当前的 ADB 连接
- 切换到另一个 IP 地址进行连接
- 检测当前 ADB 连接状态
- 返回当前连接的端口和 IP 地址

## 编译

确保你已经安装了CMake和相应的编译器。然后按照以下步骤编译项目：

1. **创建构建目录**：
   mkdir build
   cd build

2. **运行 CMake**：
    cmake..

3. **编译项目**：
    make

## 运行

编译成功后，你可以运行生成的可执行文件 ADBTest：
./ADBTest

## 注意事项

确保你的设备已经连接并且 ADB 驱动已经安装。
你可以通过修改 main.cpp 中的 IP 地址和端口来测试不同的设备。

##贡献

欢迎提交问题和建议，或者通过 Pull Request 贡献代码。

## 许可证

本项目采用 MIT License。
