
#include <stdio.h>



#include "EventLoop.h"
#include "TcpServer.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "TcpConnection.h"
#include "SocketsOps.h"

void onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        printf("onConnection(): new connection [%s] from %s\n", conn->name().c_str(), conn->peerAddress().toHostPort().c_str());
    }
    else
    {
        printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
    }
}

void onMessage(const TcpConnectionPtr& conn, const char* data, ssize_t len)
{
    printf("onMessage(): received %zd bytes from connection [%s]\n", len, conn->name().c_str());
}

int main(int argc, char **argv)
{
    printf("main(): pid = %d \n", getpid());

    InetAddress listenAddr(9981);
    EventLoop loop;

    TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
    return true;
}