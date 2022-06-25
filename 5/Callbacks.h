#ifndef MY_CHAT_CALLBACK
#define MY_CHAT_CALLBACK

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "datetime/Timestamp.h"


typedef boost::function<void()> TimerCallback;

class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef boost::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void(const TcpConnectionPtr&, const char*data, ssize_t len)> MessageCallback;

#endif //MY_CHAT_CALLBACK
