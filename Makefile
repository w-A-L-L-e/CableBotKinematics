CXX = g++
CFLAGS = -O2 -Wall

all: cable_bot_demo

cable_bot_demo:	cable_bot_kinematics.cpp
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	@rm -vf cable_bot_demo
