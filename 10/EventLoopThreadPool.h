#ifndef MY_CHAT_EVENTLOOPTHREADPOOL

#define MY_CHAT_EVENTLOOPTHREADPOOL

#include <vector>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "thread/Condition.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"


class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
    public:
        EventLoopThreadPool(EventLoop *baseLoop);
        ~EventLoopThreadPool();
        void setThreadNum(int numThreads) { numThreads_ = numThreads;}
        void start();
        EventLoop* getNextLoop();

    private:
        EventLoop* baseLoop_;
        bool started_;
        int numThreads_;
        int next_;
        boost::ptr_vector<EventLoopThread> threads_;
        std::vector<EventLoop*> loops_;
};

#endif //MY_CHAT_EVENTLOOPTHREADPOOL