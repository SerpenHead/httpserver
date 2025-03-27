#pragma once

#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
//stdexcept 是标准异常类的基类，用于处理标准异常
#include <stdexcept>
#include <cstring>
#include <iostream>
//epoll_event结构体的定义如下：
//struct epoll_event {
//    uint32_t events; /* Epoll events */
//    epoll_data_t data; /* User data variable */
//};
//data是一个联合体，可以存放fd、指针、u32、u64等数据，根据events的不同，data的含义也不同
// epoll_data联合体的定义如下：
//typedef union epoll_data {
//    void *ptr;
//    int fd;
//    uint32_t u32;
//    uint64_t u64;
//} epoll_data_t;               

class Epoll {
public:
    explicit Epoll(int maxEvents = 1024);
    ~Epoll();
    //三种基本操作
    void addFd(int fd, uint32_t events);
    void modFd(int fd, uint32_t events);
    void delFd(int fd);
    std::vector<epoll_event> wait(int timeoutMs = -1);

private:
    int epollFd;        
    std::vector<epoll_event> events;    // 就绪事件列表
};

