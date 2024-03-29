#ifndef MY_CHAT_CONNECTOR

#define MY_CHAT_CONNECTOR

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "InetAddress.h"
#include "TimerId.h"

class EventLoop;
class Channel;


class Connector
{  
    public:
        typedef boost::function<void (int sockfd)> NewConnectionCallback;

        Connector(EventLoop* loop, const InetAddress& serverAddr);
        ~Connector();

        void setNewConnectionCallback(const NewConnectionCallback& cb)
        {
            newConnectionCallback_  = cb;
        }

        void start();

        void restart();

        void stop();

        const InetAddress& serverAddress() const { return serverAddr_; }

    private:
        enum States {kDisconnected, kConnecting, kConnected};

        static const int kMaxRetryDelayMs = 30 * 1000;
        static const int kInitretryDelayMs = 500;

        void setState(States s) { state_ = s; }
        void startInLoop();
        void connect();
        void connecting(int sockfd);
        void handleWrite();
        void handleError();
        void retry(int sockfd);
        int removeAndResetChannel();
        void resetChannel();

        EventLoop* loop_;
        InetAddress serverAddr_;
        bool connect_;
        States state_;
        boost::scoped_ptr<Channel> channel_;
        NewConnectionCallback newConnectionCallback_;
        int retryDelayMs_;
        TimerId timerId_;
};

typedef boost::shared_ptr<Connector> ConnectorPtr;


#endif //MY_CHAT_CONNECTOR