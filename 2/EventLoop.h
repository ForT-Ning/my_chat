
#ifndef MY_CHAT_EVENTLOOP
#define MY_CHAT_EVENTLOOP

#include <boost/scoped_ptr.hpp>
#include <sys/syscall.h>
#include <vector>
#include <assert.h>

#include "datetime/copyable.h"
#include "TimerId.h"
#include "Callbacks.h"

extern pid_t getCtid();

class Channel;
class Poller;
class TimerQueue;

using namespace muduo;


class EventLoop : public muduo::copyable
{
private:
    /* data */
    void abortNotInLoopThread()
    {
        printf("EventLoop::abortNotInLoopThread was created in threadId_ = %d, current thread id = %d \n" , threadId_, getCtid());
    }

    bool looping_;
    const pid_t threadId_;

    typedef std::vector<Channel*> ChannelList;
    bool quit_;
    boost::scoped_ptr<Poller> poller_;
    ChannelList activeChannels_;

    boost::scoped_ptr<TimerQueue> timerQueue_;

public:
    EventLoop(/* args */);
    ~EventLoop();
    void loop();
    void quit();

    void updateChannel(Channel* channel);

    EventLoop* GetEventLoopOfCurrentThread();
    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {return threadId_ == getCtid();}

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);

    TimerId runAfter(double delay, const TimerCallback& cb);

    TimerId runEvery(double interval, const TimerCallback& cb);

};

#endif //MY_CHAT_EVENTLOOP