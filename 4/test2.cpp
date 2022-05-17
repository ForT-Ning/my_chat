#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <muduo/base/Thread.h>

#include "EventLoop.h"
//#include "thread/Thread.h"

EventLoop* g_loop;
int g_flag = 0;

void run4()
{
    printf("run4() pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->quit();
}

void run3()
{
    printf("run3() pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->runAfter(3, run4);
    g_flag = 3;
}

void run2()
{
    printf("run2() pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->queueInLoop(run3);
}

void run1()
{
    g_flag = 1;
    printf("run1() pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->runInLoop(run2);
    g_flag = 2;
}


// int main(int argc, char **argv)
// {
//     printf("main() pid = %d, flag = %d\n", getpid(), g_flag);
//     EventLoop loop;
//     g_loop = &loop;
//     loop.runAfter(2, run1);
//     loop.loop();
//     printf("main() pid = %d, flag = %d\n", getpid(), g_flag);
// }

void print() {
    printf("quit\n");
    g_loop->quit();
}

void threadFunc()
{
    g_loop->runAfter(1.0, print);
}

int main()
{
    EventLoop loop;
    g_loop = &loop;
    muduo::Thread t(threadFunc);
    t.start();
    loop.loop();
    return 0;
}