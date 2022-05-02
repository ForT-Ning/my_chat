
#ifndef MY_CHAT_EVENTLOOP
#define MY_CHAT_EVENTLOOP

#include <boost/scoped_ptr.hpp>
#include <sys/syscall.h>
#include <vector>
#include <assert.h>

#include "datetime/copyable.h"

extern pid_t getCtid();

class Channel;
class Poller;

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
};

#endif //MY_CHAT_EVENTLOOP