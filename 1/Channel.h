#ifndef MY_CHAT_CHANNEL
#define MY_CHAT_CHANNEL

#include <poll.h>
#include <boost/function.hpp>

#include "datetime/copyable.h"

class EventLoop;

class Channel : public muduo::copyable
{
    public:
        Channel(EventLoop* loop, int fd);

        typedef boost::function<void()> EventCallback;

        void handleEvent();

        void setReadCallback(const EventCallback& cb)
        {
            readCallback_ = cb;
        }

        void setWriteCallback(const EventCallback& cb)
        {
            writeCallback_ = cb;
        }

        void setErrorCallback(const EventCallback& cb)
        {
            errorCallback_ = cb;
        }

        int fd() { return fd_; }

        int events() const { return events_; }

        void set_revents(int revt) { revents_ = revt; }

        bool isNoneEvent() const { return events_ == kNoneEvent; }

        void enableReading() { events_ |= kReadEvent; update(); }

        void enableWriting() { events_ |= kWriteEvent; update(); }

        void disableWriting() { events_ &= ~kWriteEvent; update(); }

        void disableAll() { events_ = kNoneEvent; update(); }

        int index() { return index_; }

        void set_index(int idx) { index_ = idx; }

        EventLoop* ownerLoop() { return loop_; }

    private:
        void update();

        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvent;

        EventLoop* loop_;
        const int   fd_;
        int         events_;
        int         revents_;
        int         index_;

        EventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback errorCallback_;
};



#endif //MY_CHAT_CHANNEL