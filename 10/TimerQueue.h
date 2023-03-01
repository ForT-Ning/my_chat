#ifndef MY_CHAT_TIMERQUEUE
#define MY_CHAT_TIMERQUEUE

#include <utility>
#include <set>
#include <vector>

#include "Callbacks.h"
#include "TimerId.h"
#include "Channel.h"

using namespace muduo;


class Timer;
class EventLoop;

class TimerQueue
{
    public:
        TimerQueue(EventLoop* loop);
        ~TimerQueue();

        TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
        void cancel(TimerId timerId) {}

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;

        void handleRead();

        std::vector<Entry> getExpired(Timestamp now);
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;

        TimerList timers_;

        void addTimerInLoop(Timer* timer);
};



#endif //MY_CHAT_TIMERQUEUE