CC = g++
INCLUDE = include
CXXFLAGS = -Wall -std=c++17 -g -O3 -fopenmp -I$(INCLUDE)
TARGETS = sfeirvd_aon sfeirvd_leaky

all: sfeirvd_aon sfeirvd_leaky

sfeirvd_aon: lib/sfeirvd_aon.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)
sfeirvd_leaky: lib/sfeirvd_leaky.cpp
	$(CC) $^ -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGETS)
