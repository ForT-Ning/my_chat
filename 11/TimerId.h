#ifndef MY_CHAT_TIMERID
#define MY_CHAT_TIMERID

class Timer;

class TimerId
{
    public:
        TimerId(Timer* timer, int64_t seq = 0)
            :  timer_(timer),
                sequence_(seq)
        { }

        friend class TimerQueue;
    private:
        Timer* timer_;
        int64_t sequence_;
};

#endif //MY_CHAT_TIMERID