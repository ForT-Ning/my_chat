
#include <stdio.h>
#include <assert.h>
#include <boost/bind.hpp>
#include <muduo/base/Types.h>

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
    channel_->setReadCallback(boost::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(boost::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(boost::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(boost::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection()
{   }

void TcpConnection::send(const std::string& message)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            loop_->runInLoop(boost::bind(&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string& message)
{
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrote = ::write(channel_->fd(), message.data(), message.size());
        if( nwrote >= 0)
        {
            if(implicit_cast<size_t>(nwrote) < message.size())
            {
                printf("going to write more data\n");
            }
        }
        else
        {
            nwrote = 0;
            if(errno != EWOULDBLOCK)
            {
                printf("TcpConnection::sendInLoop\n");
            }
        }
    }
    assert(nwrote >= 0);
    if(implicit_cast<size_t>(nwrote) < message.size())
    {
        outputBuffer_.append(message.data() + nwrote, message.size()-nwrote);
        if(!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setState(kDisconnecting);
        loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop,this));
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if(!channel_->isWriting())
    {
        socket_->shutdownWrite();
    }
}

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
    assert(state_ == kConnected || state_ == kDisconnecting);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(get_pointer(channel_));
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    //-----No Timestamp
    // char buf[65536];
    // ssize_t n = ::read(channel_->fd(), buf, sizeof buf);
    // if( n > 0)
    // {
    //     messageCallback_(shared_from_this(), buf, n);
    // }
    // else if(n == 0)
    // {
    //     handleClose();
    // }
    // else
    // {
    //     handleError();
    // }
    int savedError = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedError);
    if(n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if( n ==0)
    {
        handleClose();
    }
    else
    {
        errno = savedError;
        handleError();
    }

}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();
    if(channel_->isWriting())
    {
        size_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if(n > 0)
        {
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting();
                if(state_ == kDisconnecting)
                {
                    shutdownInLoop();
                }
            }
            else
            {
                printf("I am going to write more data\n");
            }
        }
        else
        {
            printf("TcpConnection::handleWrite\n");
        }
    }
    else
    {
        printf("Connection is down. no more writing\n");
    }
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnected || state_ == kDisconnecting);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(channel_->fd());
}

