#ifndef MY_CHAT_EVENTLOOPTHREAD

#define MY_CHAT_EVENTLOOPTHREAD

#include "EventLoop.h"
#include "./thread/Thread.h"
#include "./thread/Condition.h"

class EventLoopThread
{
    public:
        EventLoopThread();
        ~EventLoopThread();
        EventLoop* startLoop();

    private:
        void threadFunc();

        EventLoop* loop_;
        bool exiting_;
        Thread thread_;
        MutexLock mutex_;
        Condition cond_;
};

#endif //MY_CHAT_EVENTLOOPTHREAD