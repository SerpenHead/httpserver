#include "socket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#define BACKLOG 1024

Socket::Socket() : listenFd(-1) {}

Socket::~Socket() {
    closeSocket();
}

bool Socket::createSocket(int port) {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == -1) {
        std::cerr << "Failed to create socket.\n";
        return false;
    }

    int opt = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(listenFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed.\n";
        return false;
    }

    if (listen(listenFd, BACKLOG) == -1) {
        std::cerr << "Listen failed.\n";
        return false;
    }

    setNonBlocking(listenFd);
    std::cout << "Server listening on port " << port << "...\n";
    return true;
}

// 设置非阻塞
void Socket::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 接受新连接
int Socket::acceptConnection() {
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(listenFd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd >= 0) {
        setNonBlocking(clientFd);
    }
    return clientFd;
}

void Socket::closeSocket() {
    if (listenFd != -1) {
        close(listenFd);
        listenFd = -1;
    }
}