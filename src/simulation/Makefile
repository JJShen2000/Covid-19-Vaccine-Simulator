CC = g++
CXXFLAGS = -Wall -std=c++14 -g -O3
TARGETS = all_or_nothing leaky

all: all_or_nothing leaky

all_or_nothing: run/run_sfeirvd_aon.cpp Node.cpp ContactGroup.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)
leaky: run/run_sfeirvd_leaky.cpp Node.cpp ContactGroup.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGETS)