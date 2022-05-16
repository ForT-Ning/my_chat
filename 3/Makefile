LIB_SRC = Channel.cpp EventLoop.cpp Poller.cpp Timer.cpp TimerQueue.cpp EventLoopThread.cpp ./datetime/Timestamp.cc ./thread/Thread.cc
BINARIES =  test3

all: $(BINARIES)

CXXFLAGS = -O0 -g -Wall -I ../.. -pthread 

MUDUO = -lmuduo_base

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cpp, $^) $(MUDUO)

clean:
	rm -f $(BINARIES) core

#main: main.cpp
#test1: test1.cpp
test3: test3.cpp