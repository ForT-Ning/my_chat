#ifndef MY_CHAT_ACCEPTOR

#define MY_CHAT_ACCEPTOR

#include <boost/function.hpp>

#include "Socket.h"
#include "Channel.h"

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
        void handelRead();

        EventLoop* loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        newConnectionCallback newConnectionCallback_;
        bool listenning_;
};


#endif //MY_CHAT_ACCEPTOR