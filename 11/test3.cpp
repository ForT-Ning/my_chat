#include <stdio.h>


#include "EventLoop.h"
#include "TcpServer.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "TcpConnection.h"
#include "SocketsOps.h"

std::string message;

void onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        printf("onConnection(): new connection [%s] from %s\n", conn->name().c_str(), conn->peerAddress().toHostPort().c_str());
        conn->send(message);
    }
    else
    {
        printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
    }
}

void onWriteComplete(const TcpConnectionPtr& conn)
{
    conn->send(message);
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buff, Timestamp receiveTime)
{
    printf("onMessage(): received %zd bytes from connection [%s] at %s\n", 
            buff->readableBytes(), 
            conn->name().c_str(),
            receiveTime.toFormattedString().c_str());
    //printf("onMessage(): [%s]\n", buff->retrieveAsString().c_str());
    buff->retrieveAll();
}

int main(int argc, char **argv)
{
    printf("main(): pid = %d \n", getpid());

    std::string line;
    for(int i = 33; i < 127; ++i)
    {
        line.push_back(char(i));
    }
    line += line;

    for(size_t i = 0; i < 127 - 33; ++i)
    {
        message += line.substr(i, 72) + '\n';
    }

    InetAddress listenAddr(9981);
    EventLoop loop;

    TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setWriteCompleteCallback(onWriteComplete);
    if(argc > 1)
    {
        server.setThreadNum(atoi(argv[1]));
    }
    server.start();

    loop.loop();
    return true;
}