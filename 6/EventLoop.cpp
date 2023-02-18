#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <sys/eventfd.h>

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include "CurrentID.h"
#include "TimerQueue.h"


__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0)
    {
        printf("Error: failed in eventfd");
        abort();
    }
    return evtfd;
}

EventLoop::EventLoop(/* args */) : 
    looping_(false), 
    quit_(false), 
    callingPendingFunctors_(false),
    threadId_(getCtid()), 
    poller_(new Poller(this)), 
    timerQueue_(new TimerQueue(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_))
{
    //printf("EventLoop created: %d \n", threadId_);
    if(t_loopInThisThread)
    {
        ;//printf("Another EvenLoop %d exists in this thread\n", threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    ::close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}

EventLoop* EventLoop::GetEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;


    // ::poll(NULL, 0, 5*1000);

    // printf("EventLoop stop looping: %d\n", threadId_);
    // looping_ = false;
    while (!quit_)
    {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent();
        }
        doPendingFunctors();
    }
    //printf("EventLoop stop looping: %d\n", threadId_);
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    if(!isInLoopThread())
        wakeup();
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::runInLoop(const Functor& cb)
{
    if(isInLoopThread())
        cb();
    else
        queueInLoop(cb);
}

void EventLoop::queueInLoop(const Functor& cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i = 0; i < functors.size(); ++i)
    {
        functors[i]();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::wakeup()
{
    uint64_t one = -1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof one);
    if( n != sizeof one)
    {
        ;//printf("Error: EventLoop::wakeup() writes %d bytes instead of 8\n");
    }
}

void EventLoop::handleRead()
{
    uint64_t one = -1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof one);
    if( n != sizeof one)
    {
        ;//printf("Error: EventLoop::wakeup() writes %d bytes instead of 8\n");
    }
}