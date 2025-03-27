#include "socket.h"

// 构造函数：创建、绑定、设置非阻塞
Socket::Socket(int port) {
    // 创建 socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    // 绑定端口
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listenFd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        close(listenFd);
        throw std::runtime_error("Bind failed");
    }
}

// 析构函数：关闭 socket
Socket::~Socket() {
    close(listenFd);
}

// 监听 socket
void Socket::listen(int backlog) {
    if (::listen(listenFd, backlog) == -1) {
        throw std::runtime_error("Listen failed");
    }
}

// 接受客户端连接
int Socket::acceptConnection() {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(listenFd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd == -1) {
        throw std::runtime_error("Accept failed");
    }
    return clientFd;
}

// 设置 fd 为非阻塞模式
void Socket::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
