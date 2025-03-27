#include "epoll.h"


// 构造函数：创建 epoll 实例
Epoll::Epoll(int maxEvents) : events(maxEvents) {
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
}

// 析构函数：关闭 epoll fd
Epoll::~Epoll() {
    close(epollFd);
}

// 添加 fd
void Epoll::addFd(int fd, uint32_t events) {
    epoll_event event{};
    //这里联合体data存放的数据就是客户端的fd
    event.data.fd = fd;
    event.events = events;
    //ctl函数执行EPOLL_CTL_ADD操作，将fd添加到epoll实例中
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
        throw std::runtime_error("Failed to add fd to epoll");
    }
}

// 修改 fd
void Epoll::modFd(int fd, uint32_t events) {
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) == -1) {
        throw std::runtime_error("Failed to modify fd in epoll");
    }
}

// 删除 fd
void Epoll::delFd(int fd) {
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        throw std::runtime_error("Failed to delete fd from epoll");
    }
}

// 等待事件
// timeoutMs为-1：阻塞等待，直到有事件发生, 为0：立即返回,  >0：等待指定时间
std::vector<epoll_event> Epoll::wait(int timeoutMs) {
    int numEvents = epoll_wait(epollFd, events.data(), events.size(), timeoutMs);
    if (numEvents == -1) {
        throw std::runtime_error("epoll_wait error: " + std::string(strerror(errno)));
    }
    return std::vector<epoll_event>(events.begin(), events.begin() + numEvents);
}
