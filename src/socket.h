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
        Socket();
        ~Socket();
    
        bool createSocket(int port);    // 创建监听 socket
        void setNonBlocking(int fd);    // 设置非阻塞
        int acceptConnection();         // 接受新连接
        void closeSocket();             // 关闭 socket
        int getListenFd() const { return listenFd; }
    
    private:
        int listenFd;
    };

