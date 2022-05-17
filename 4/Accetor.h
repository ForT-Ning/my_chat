#ifndef MY_CHAT_ACCEPTOR

#define MY_CHAT_ACCEPTOR

#include <boost/function.hpp>

class InetAddress;
class EventLoop;

class Acceptor
{
    public:
        typedef boost::function<void(int sockfd, const InetAddress&)> newConnectionCallback;

        Acceptor(EventLoop* loop, const InetAddress& listenAddr);

        void setNewConnectionCallback(const newConnectionCallback& cb)
        {
            newConnectionCallback_ = cb;
        }

        bool listenning() const { return listenning_;}
        void listen();

    private:
        newConnectionCallback newConnectionCallback_;
        bool listenning_;
};


#endif //MY_CHAT_ACCEPTOR