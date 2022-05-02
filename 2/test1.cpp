#include <stdio.h>
#include <boost/bind.hpp>

#include "EventLoop.h"
#include "Channel.h"

#define GET_TIME_STR muduo::Timestamp::now().toString().c_str()

EventLoop* g_loop;

int cnt = 0;


void printfTid()
{
    printf("pid = %d, tid = %d\n", getpid(), getCtid());
    printf("now %s\n", GET_TIME_STR);
}

void print(const char* msg)
{
    printf("msg %s %s\n", GET_TIME_STR, msg);
    if(++cnt == 20)
    {
        g_loop->quit();
    }
}

int main(int argc, char* argv[])
{
    printfTid();
    EventLoop loop;
    g_loop = &loop;

    printf("main");
    loop.runAfter(1, boost::bind(print, "once1"));
    loop.runAfter(1.5, boost::bind(print, "once1.5"));
    loop.runAfter(2.5, boost::bind(print, "once2.5"));
    loop.runAfter(3.5, boost::bind(print, "once3.5"));
    loop.runEvery(2, boost::bind(print, "every2"));
    loop.runEvery(3, boost::bind(print, "every3"));

    loop.loop();
    print("main loop exits");
    sleep(1);
}