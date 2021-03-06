#include <sys/timerfd.h>
#include <stdio.h>

#include "EventLoop.h"
#include "Channel.h"

EventLoop* g_loop;

void timeout()
{
    printf("Timeout!\n");
    g_loop->quit();
}

//g++ -o test1 test1.cpp EventLoop.cpp Channel.cpp Poller.cpp ./datetime/Timestamp.cc -lpthread
//g++ -g test1.cpp EventLoop.cpp Channel.cpp Poller.cpp ./datetime/Timestamp.cc -lpthread

int main(int argc,char* argv[])
{
    EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);

    return 0;
}