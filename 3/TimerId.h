#ifndef MY_CHAT_TIMERID
#define MY_CHAT_TIMERID

class Timer;

class TimerId
{
    public:
        explicit TimerId(Timer* timer)
            :  value_(timer)
        { }
    private:
        Timer* value_;
};

#endif //MY_CHAT_TIMERID