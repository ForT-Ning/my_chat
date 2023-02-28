LIB_SRC = 	Channel.cpp	EventLoop.cpp 	Poller.cpp		Timer.cpp	TimerQueue.cpp	EventLoopThread.cpp	Acceptor.cpp	InetAddress.cpp \
		 	Socket.cpp 	SocketsOps.cpp	TcpServer.cpp 	TcpConnection.cpp 			Buffer.cpp\
			./datetime/Timestamp.cc 	./thread/Thread.cc
BINARIES = test1 test2 test3

all: $(BINARIES)

#CXXFLAGS = -O0 -g -Wall -I ../.. -pthread 

CXXFLAGS = -g -pthread 

MUDUO = -lmuduo_base

# $(BINARIES):
# 	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cpp, $^) $(MUDUO)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cpp, $^) $(MUDUO)

clean:
	rm -f $(BINARIES) core

test1: test1.cpp
test2: test2.cpp
test3: test3.cpp