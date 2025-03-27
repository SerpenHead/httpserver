#pragma once
#include <vector>

class EpollWrapper {
public:
    EpollWrapper();
    ~EpollWrapper();

    bool init(int maxEvents = 1024);
    bool addFd(int fd, bool enableET = true);
    bool removeFd(int fd);
    bool modifyFd(int fd, uint32_t events);
    int wait(int timeoutMs = -1);

    int getEventFd(int index) const;
    uint32_t getEventType(int index) const;

private:
    int epollFd;
    std::vector<struct epoll_event> events;
};


