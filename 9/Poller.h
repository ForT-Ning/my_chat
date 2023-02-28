#ifndef MY_CHAT_POLLER
#define MY_CHAT_POLLER

#include <vector>
#include <map>
#include <poll.h>
#include <stdio.h>

#include "datetime/copyable.h"
#include "EventLoop.h"

using namespace muduo;

class Channel;

class Poller : public muduo::copyable
{
    public:
        typedef std::vector<Channel*> ChannelList;

        Poller(EventLoop* loop);
        ~Poller();

        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        void updateChannel(Channel* channel);

        void removeChannel(Channel* channel);

        void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }
    
    private:
        void fillActiveChannels(int numEvents, ChannelList* activeChannel) const;

        typedef std::vector<struct pollfd> PollFdList;
        typedef std::map<int, Channel*> ChannelMap;

        EventLoop* ownerLoop_;
        PollFdList pollfds_;
        ChannelMap channels_;
};


#endif //MY_CHAT_POLLER