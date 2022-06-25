LIB_SRC = Channel.cpp EventLoop.cpp Poller.cpp Timer.cpp TimerQueue.cpp EventLoopThread.cpp Acceptor.cpp InetAddress.cpp \
		 	Socket.cpp SocketsOps.cpp 	TcpServer.cpp TcpConnection.cpp\
			./datetime/Timestamp.cc ./thread/Thread.cc
BINARIES =  test5

all: $(BINARIES)

#CXXFLAGS = -O0 -g -Wall -I ../.. -pthread 

CXXFLAGS = -g -pthread 

MUDUO = -lmuduo_base

# $(BINARIES):
# 	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cpp, $^) $(MUDUO)

$(BINARIES):
	g++ $(CXXFLAGS) $(LIB_SRC) $(filter %.cpp, $^) $(MUDUO)

clean:
	rm -f $(BINARIES) core

#main: main.cpp
#test1: test1.cpp
#test3: test3.cpp
#test4: test4.cpp
test5: test5.cpp