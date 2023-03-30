#include <stdio.h>

#include "Connector.h"
#include "EventLoop.h"

EventLoop* g_loop;

using namespace muduo;

void connectionCallback(int sockfd)
{
    printf("Connected.\n");
    g_loop->quit();
}

int main(int argc, char* argv[])
{
    EventLoop loop;
    g_loop = &loop;
    InetAddress addr("127.0.0.1", 9981);
    ConnectorPtr connector(new Connector(&loop, addr));
    connector->setNewConnectionCallback(connectionCallback);
    connector->start();

    loop.loop(); 
}