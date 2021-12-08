CC = g++
CXXFLAGS = -Wall -std=c++14 -g -O3
TARGETS = sfeirvd_aon sfeirvd_leaky

all: sfeirvd_aon sfeirvd_leaky

sfeirvd_aon: run/run_sfeirvd_aon.cpp Node.cpp ContactGroup.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)
sfeirvd_leaky: run/run_sfeirvd_leaky.cpp Node.cpp ContactGroup.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGETS)