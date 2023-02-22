#ifndef MY_CHAT_TCPSERVER

#define MY_CHAT_TCPSERVER

#include <map>
#include <boost/scoped_ptr.hpp>

#include "Callbacks.h"
#include "Acceptor.h"

class EventLoop;
class InetAddress;

class TcpServer
{
private:
    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop *loop_;
    const std::string name_;
    boost::scoped_ptr<Acceptor> acceptor_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;

public:

    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();

    void start();

    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_=cb; }

};


#endif //MY_CHAT_TCPSERVER