
#include <stdio.h>
#include <assert.h>
#include <boost/bind.hpp>

#include "TcpConnection.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Channel.h"
#include "SocketsOps.h"


TcpConnection::TcpConnection(EventLoop* loop,
                             const std::string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    :   loop_(loop),
        name_(nameArg),
        state_(kConnecting),
        socket_(new Socket(sockfd)),
        channel_(new Channel(loop, sockfd)),
        localAddr_(localAddr),
        peerAddr_(peerAddr)
{
    channel_->setReadCallback(boost::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(boost::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(boost::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(boost::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection()
{   }

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(get_pointer(channel_));
}

void TcpConnection::handleRead()
{
    char buf[65536];
    ssize_t n = ::read(channel_->fd(), buf, sizeof buf);
    if( n > 0)
    {
        messageCallback_(shared_from_this(), buf, n);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        handleError();
    }
}

void TcpConnection::handleWrite()
{

}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(channel_->fd());
}

