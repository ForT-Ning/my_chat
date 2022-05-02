#include "EventLoop.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <boost/function.hpp>

#include "Channel.h"
#include "Poller.h"
#include "CurrentID.h"

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop(/* args */) : 
    looping_(false), quit_(false), threadId_(getCtid()), poller_(new Poller(this))
{
    printf("EventLoop created: %d \n", threadId_);
    if(t_loopInThisThread)
    {
        printf("Another EvenLoop %d exists in this thread\n", threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);

}

void EventLoop::updateChannel(Channel* channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
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
        poller_->poll(kPollTimeMs, &activeChannels_);
        for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
    printf("EventLoop stop looping: %d\n", threadId_);
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
}


