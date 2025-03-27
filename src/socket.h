#pragma once
//arpa/inet.h头文件是用于处理IP地址转换的函数
#include <arpa/inet.h>
//unistd.h头文件是用于处理Unix系统调用的函数
#include <unistd.h>
//fcntl.h头文件是用于处理文件控制操作的函数
#include <fcntl.h>
#include <stdexcept>
#include <string>

class Socket {
public:
    explicit Socket(int port);
    ~Socket();
    void listen(int backlog = 5);
    int acceptConnection();
    int getFd() const { return listenFd; }
    static void setNonBlocking(int fd);

private:
    int listenFd;       // 监听 socket fd
    sockaddr_in addr;   // 服务器地址信息
};

