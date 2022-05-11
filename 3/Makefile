LIB_SRC = Channel.cpp EventLoop.cpp Poller.cpp Timer.cpp TimerQueue.cpp ./datetime/Timestamp.cc
BINARIES =  test2

all: $(BINARIES)

CXXFLAGS = -O0 -g -Wall -I ../.. -pthread

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cpp, $^)

clean:
	rm -f $(BINARIES) core

#main: main.cpp
#test1: test1.cpp
test2: test2.cpp