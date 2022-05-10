#ifndef MY_CHAT_TIMER
#define MY_CHAT_TIMER

#include "Callbacks.h"
using namespace muduo;


class Timer
{
    private:
        const TimerCallback callback_;
        Timestamp expiration_;
        const double interval_;
        const bool repeat_;
    public:
        Timer(const TimerCallback& cb, Timestamp when, double interval)
            : callback_(cb), expiration_(when), interval_(interval), repeat_(interval > 0.0)
        { }

        void run() const { callback_(); }

        Timestamp expiration() const { return expiration_; }
        bool repeat() const { return repeat_; }

        void restart(Timestamp now);
        
};

#endif //MY_CHAT_TIMER