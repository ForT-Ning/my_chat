#include <muduo/base/Types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "SocketsOps.h"

using namespace muduo;

namespace
{
    typedef struct sockaddr SA;

    const SA* sockaddr_cast(const struct sockaddr_in* addr)
    {
        return static_cast<const SA*>(implicit_cast<const void*>(addr));
    }

    SA* sockaddr_cast(struct sockaddr_in* addr)
    {
        return static_cast<SA*>(implicit_cast<void*>(addr));
    }

    void setNonBlockAndCloseOnExec(int sockfd)
    {
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int ret = ::fcntl(sockfd, F_SETFL, flags);

        flags = ::fcntl(sockfd, F_GETFD, 0);
        flags |= FD_CLOEXEC;
        ret = ::fcntl(sockfd, F_SETFD, flags);
    }
}

int sockets::createNonblockingOrDie()
{
#if VALGRIND
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0)
    {
        printf("erron in createNonblockingOrDie() 1.\n");
        exit(1);
    }
    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0)
    {
        printf("erron in createNonblockingOrDie() 2.\n");
        exit(1);
    }
#endif // VALGRIND
    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
    int ret = ::bind(sockfd, sockaddr_cast(&addr), sizeof addr);
    if( ret < 0)
    {
        printf("erron in bindOrDie() \n");
        exit(1);
    }
}

void sockets::listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if(ret < 0)
    {
        printf("erron in listenOrDie() \n");
        exit(1);
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof *addr;
#if VALGRIND
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if(connfd < 0)
    {
        int savedErrno = errno; // savedError EAFAIN ECONNABORTED
        printf("%d erron in accept() \n", savedErrno);
        exit(1);
    }
    return connfd;
}

void sockets::close(int sockfd)
{
    if(::close(sockfd) < 0)
    {
        printf("erron in close() \n");
        exit(1);
    }
}

void sockets::toHostPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
    char host[INET_ADDRSTRLEN] = "INVALID";
    ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
    uint64_t port = sockets::networkToHost16(addr.sin_port);
    snprintf(buf, size, "%s:%u", host, port);
}

void sockets::fromHostPort(const char *ip, uint16_t port, struct sockaddr_in* addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = hostToNetwork16(port);
    if(::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        printf("erron in formHostPort() \n");
        exit(1);
    }
}