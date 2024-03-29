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
        void cancel(TimerId timerId);

    private:
        typedef std::pair<Timestamp, Timer*> Entry;
        typedef std::set<Entry> TimerList;
        typedef std::pair<Timer*, int64_t> ActiveTimer;
        typedef std::set<ActiveTimer> ActiveTimerSet;
        
        void addTimerInLoop(Timer* timer);
        void cancelInLoop(TimerId timerId);

        void handleRead();

        std::vector<Entry> getExpired(Timestamp now);
        void reset(const std::vector<Entry>& expired, Timestamp now);

        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;

        TimerList timers_;

        bool callingExpiredTimers_;
        ActiveTimerSet activeTimers_;
        ActiveTimerSet cancelingTimers_;
};


#endif //MY_CHAT_TIMERQUEUE