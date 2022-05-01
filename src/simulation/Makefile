CC = g++
CXXFLAGS = -Wall -std=c++17 -g -O3 -fopenmp

exe = main
obj = main.o BaseModel.o ContactGroup.o ContactMatrix.o ExpiringState.o Heap.o Node.o Random.o Simulation.o StatisticsAgedGeoStatePopulation.o Time.o

# TARGETS = sfeirvd_aon sfeirvd_leaky

# all: sfeirvd_aon sfeirvd_leaky

all: $(obj)
	$(CC) -o $(exe) $(obj) $(CXXFLAGS)
# main: main.o
# 	$(CC) $^ -o $@ $(CXXFLAGS)
%.o: %.c
	$(CC) -c $^ -o $@ $(CXXFLAGS)
# main.o: main.cpp StatisticsAgedGeoStatePopulation.o
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# BaseModel.o: Node.o ContactGroup.o BaseModel.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# ContactGroup.o: ContactGroup.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# ContactMatrix.o: ContactMatrix.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# ExpiringState.o: ExpiringState.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# Heap.o: Heap.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# Node.o: Node.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# Random.o: Random.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# Simulation.o: ExpiringState.o BaseModel.o Heap.o Simulation.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# Time.o: Time.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)
# StatisticsAgedGeoStatePopulation.o: Simulation.o StatisticsAgedGeoStatePopulation.cpp
# 	$(CC) -c $^ -o $@ $(CXXFLAGS)

# sfeirvd_aon: run/run_sfeirvd_aon.cpp
# 	$(CC) $^ -o $@ $(CXXFLAGS)
# sfeirvd_leaky: run/run_sfeirvd_leaky.cpp
# 	$(CC) $^ -o $@ $(CXXFLAGS)

.PHONY: clean
clean: 
	rm -f $(obj)
