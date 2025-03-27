#include "epoll.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

EpollWrapper::EpollWrapper() : epollFd(-1) {}

EpollWrapper::~EpollWrapper() {
    if (epollFd != -1) {
        close(epollFd);
    }
}

// 初始化 epoll
bool EpollWrapper::init(int maxEvents) {
    epollFd = epoll_create(1);
    if (epollFd == -1) {
        std::cerr << "Failed to create epoll.\n";
        return false;
    }
    events.resize(maxEvents);
    return true;
}

// 添加事件
bool EpollWrapper::addFd(int fd, bool enableET) {
    struct epoll_event ev{};
    ev.data.fd = fd;
    ev.events = EPOLLIN | (enableET ? EPOLLET : 0);
    return epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) != -1;
}

// 删除事件
bool EpollWrapper::removeFd(int fd) {
    return epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) != -1;
}

// 修改事件
bool EpollWrapper::modifyFd(int fd, uint32_t events) {
    struct epoll_event ev{};
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev) != -1;
}

// 等待事件
int EpollWrapper::wait(int timeoutMs) {
    return epoll_wait(epollFd, events.data(), events.size(), timeoutMs);
}

// 获取事件的 fd
int EpollWrapper::getEventFd(int index) const {
    return events[index].data.fd;
}

// 获取事件类型
uint32_t EpollWrapper::getEventType(int index) const {
    return events[index].events;
}
