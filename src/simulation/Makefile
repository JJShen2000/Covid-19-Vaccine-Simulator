CC = g++
INCLUDE = include
CXXFLAGS = -Wall -std=c++17 -g -O3 -fopenmp -I$(INCLUDE)
TARGETS = simulator
SRC = $(wildcard lib/*.cpp)
OBJ = $(addprefix build/, $(notdir $(SRC:.cpp=.o)))

all: $(TARGETS)

$(TARGETS): $(OBJ)
	$(CC) $^ -o $@ $(CXXFLAGS)
build/%.o: lib/%.cpp
	@if [ ! -d "./build" ]; then mkdir build; fi
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGETS) $(OBJ)
	rm -rf build
