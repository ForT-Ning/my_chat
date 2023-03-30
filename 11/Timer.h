#ifndef MY_CHAT_TIMER
#define MY_CHAT_TIMER

#include "Callbacks.h"
#include "thread/Atomic.h"
#include "datetime/Timestamp.h"

using namespace muduo;


class Timer
{

    public:
        Timer(const TimerCallback& cb, Timestamp when, double interval)
            : callback_(cb), 
            expiration_(when), 
            interval_(interval), 
            repeat_(interval > 0.0),
            sequence_(s_numCreated_.incrementAndGet())
        { }

        void run() const { callback_(); }

        Timestamp expiration() const { return expiration_; }
        bool repeat() const { return repeat_; }
        int64_t sequence() const { return sequence_;}

        void restart(Timestamp now);

    private:
        const TimerCallback callback_;
        Timestamp expiration_;
        const double interval_;
        const bool repeat_;
        const int64_t sequence_;

        static AtomicInt64 s_numCreated_;
};

#endif //MY_CHAT_TIMER