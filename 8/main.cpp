#include "EventLoop.h"
#include "Channel.h"

void* threadFunc(void *arg)
{
    printf("threadFunc(): pid = %d, tid = %d\n", getpid(), getCtid());
    EventLoop loop;
    loop.loop();
}

int main(int argv, char** argc)
{
    printf("main(): pid = %d, tid = %d\n", getpid(), getCtid());
    EventLoop loop;

    pthread_t thid;
    if(pthread_create(&thid, NULL, threadFunc, NULL) != 0)
    {
        printf("thread create failua\n");
        exit(1);
    }


    loop.loop();
    pthread_exit(NULL);
}