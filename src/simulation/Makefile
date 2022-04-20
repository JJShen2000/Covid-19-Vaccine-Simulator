CC = g++
INCLUDE = include
CXXFLAGS = -Wall -std=c++17 -g -O3 -fopenmp -I$(INCLUDE)
TARGETS = aon_simulation leaky_simulation
SRC = $(wildcard lib/*.cpp)
OBJ = $(addprefix build/, $(notdir $(SRC:.cpp=.o)))

all: $(TARGETS)

aon_simulation: $(filter-out build/leaky_simulation.o, $(OBJ))
	$(CC) $^ -o $@ $(CXXFLAGS)
leaky_simulation: $(filter-out build/aon_simulation.o, $(OBJ))
	$(CC) $^ -o $@ $(CXXFLAGS)
build/%.o: lib/%.cpp
	@if [ ! -d "./build" ]; then mkdir build; fi
	$(CC) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGETS) $(OBJ)
	rm -rf build
