
#include <errno.h>
#include <boost/bind.hpp>
#include <sys/types.h>


#include "Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"
#include "Connector.h"
#include "logging/Logging.h"

using namespace muduo;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
    :loop_(loop),
     serverAddr_(serverAddr),
     connect_(false),
     state_(kDisconnected),
     timerId_(NULL),
     retryDelayMs_(kInitretryDelayMs)
{
}

Connector::~Connector()
{
    loop_->cancel(timerId_);
    assert(!channel_);
}

void Connector::start()
{
    connect_ = true;
    loop_->runInLoop(boost::bind (&Connector::startInLoop, this));
}

void Connector::startInLoop()
{
    loop_->assertInLoopThread();
    assert(state_ == kDisconnected);
    if(connect_)
    {
        connect();
    }
    else
    {
        printf("do not connect\n");
    }
}

void Connector::connect()
{
    int sockfd = sockets::createNonblockingOrDie();
    int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
    int saveErrno = (ret == 0) ? 0 : errno;
    switch (saveErrno)
    {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        connecting(sockfd);
        break;
    
    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        retry(sockfd);
        break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        printf("connect error in connector::startInLoop: %d\n", saveErrno);
        sockets::close(sockfd);
        break;

    default:
        printf("Unexpected error in connector::startInLoop: %d\n", saveErrno);
        sockets::close(sockfd);
        break;
    }

}

void Connector::restart()
{
    loop_->assertInLoopThread();
    setState(kDisconnected);
    retryDelayMs_ = kInitretryDelayMs;
    connect_ = true;
    startInLoop();
}

void Connector::stop()
{
    connect_ = false;
    loop_->cancel(timerId_);
}

void Connector::connecting(int sockfd)
{
    setState(kConnecting);
    assert(!channel_);
    channel_.reset(new Channel(loop_, sockfd));
    channel_->setWriteCallback(boost::bind(&Connector::handleWrite, this));
    channel_->setErrorCallback(boost::bind(&Connector::handleError, this));

    channel_->enableWriting();
}

int Connector::removeAndResetChannel()
{
    channel_->disableAll();
    loop_->removeChannel(get_pointer(channel_));
    int sockfd = channel_->fd();
    loop_->queueInLoop(boost::bind(&Connector::resetChannel, this));
    return sockfd;
}

void Connector::resetChannel()
{
    channel_.reset();
}

void Connector::handleWrite()
{
    if(state_ == kConnecting)
    {
        int sockfd = removeAndResetChannel();
        int err = sockets::getSocketError(sockfd);
        if(err)
        {
            printf("Connectot::handleWrite - SO_ERROR = %d (%s)\n", err, strerror_tl(err));
        }
        else if(sockets::isSelfConnect(sockfd))
        {
            printf("Connector::handleWrite = self connect\n");
            retry(sockfd);
        }
        else
        {
            setState(kConnected);
            if(connect_)
            {
                newConnectionCallback_(sockfd);
            }
            else
            {
                sockets::close(sockfd);
            }
        }
    }
    else
    {
        assert(state_ == kDisconnected);
    }
}

void Connector::handleError()
{
    assert(state_ == kConnecting);

    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    printf("SO_ERROR = %d (%s)\n", err, strerror_tl(err));
    retry(sockfd);
}

void Connector::retry(int sockfd)
{
    sockets::close(sockfd);
    setState(kDisconnected);
    if(connect_)
    {
        printf("Conenctor::retry - Retry connecting to %s int %d milliseconds. \n", serverAddr_.toHostPort(), retryDelayMs_);
        timerId_ = loop_->runAfter(retryDelayMs_/1000.0, boost::bind(&Connector::startInLoop, this));
        retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
    }
    else
    {
        printf("do not connect\n");
    }
} 