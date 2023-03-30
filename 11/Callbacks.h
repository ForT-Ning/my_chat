#ifndef MY_CHAT_CALLBACK
#define MY_CHAT_CALLBACK

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "datetime/Timestamp.h"
using namespace muduo;

class TcpConnection;
class Buffer;

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef boost::function<void()> TimerCallback;

typedef boost::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void(const TcpConnectionPtr&, Buffer* buf, Timestamp)> MessageCallback;
typedef boost::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
typedef boost::function<void(const TcpConnectionPtr&)> CloseCallback;

#endif //MY_CHAT_CALLBACK
