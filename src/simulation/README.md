# Simulation Module

## Usage

### Test
run `g++ ./test/test_SFEIRVD.cpp Node.cpp ContactGroup.cpp` with `C++11` or newer

#### Input
The test reads the following data

- `./data/graph_10x10.txt`
- `./data/param_SFEIRVD.txt`
- `./data/init_SFEIRVD.txt`
- `./vacc_SFEIRVD.txt`

#### Output
It outputs the result to `./data/statistic_10x10.csv`

#### Show to Screen

Comment `#define DETAIL` in `./SFEIRVDSimulation.hpp` at line 6 for lower loading