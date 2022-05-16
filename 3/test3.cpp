#include <stdio.h>

#include "EventLoopThread.h"

void runInThread()
{
    printf("runInThread(): pid = %d, tid = %d\n", getpid(), getCtid());
}

int main(int argc, char **argv[])
{
    printf("main(): pid = %d, tid = %d\n", getpid(), getCtid());
    EventLoopThread loopThread;
    EventLoop* loop = loopThread.startLoop();
    loop->runInLoop(runInThread);
    sleep(1);
    loop->runAfter(2, runInThread);
    sleep(10);
    loop->quit();
    printf("exit main().\n");
}