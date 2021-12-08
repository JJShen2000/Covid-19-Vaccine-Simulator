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

### Run

- at `./src/simulation`
- build all-or-nothing model exe
    - run `g++ ./run/run_sfeirvd_aon.cpp Node.cpp ContactGroup.cpp -o sfeirvd_aon`
- build leaky model exe
    - run `g++ ./run/run_sfeirvd_leaky.cpp Node.cpp ContactGroup.cpp -o sfeirvd_leaky`
- clean
    - run `rm *.gch`
- run all-or-nothing model
    - run `./sfeirvd_aon $graph_file $param_file $init_file $vacc_file $otuput_file`
- run leaky model
    - run `./sfeirvd_leaky $graph_file $param_file $init_file $vacc_file $output_file`


## File Format

### Graph File

#### Format

```
N_nd N_pr N_gp N_ag N_cm N_lc
cg[0] cg[1] ... cg[N_gp-1]
cm[0, 0, 0] ...
ag[0] lc[0] 
n_gp[0, 0] gp[0, 0] ... gp[0, n_gp[0]-1]
n_gp[1, 0] gp[0, ]
.
.
.
```

#### Explanation

- first line
    - `N_nd`: # of nodes
    - `N_pr`: # of periods per day
    - `N_gp`: # of contact groups
    - `N_ag`: # of age groups
    - `N_cm`: # of contact matrix
    - `N_lc`: # of 鄉鎮市區
- second line
    - `cg[i]` contact matrix to use for group `i`
- following `N_cm` blocks, each `N_ag^2` floating point numbers
- following `N_nd` blocks, each block denotes a node `i`
    - first line
        - age group and location group of node `i`
    - following `N_period` lines
        - first is the # of groups in period `j`
        - then the contact groups
- - this is supposed to be constructed by initalization modeule

### Param File

Includes int or floating-point number parameters. The followings are necessary for SFEIRVD model(both all-or-nothing or leaky)

#### Format

```
param_name1 num1
param_name2 num2
param_name3 num3
.
.
.
```

#### Explanation

- prob_transmission 
    - transmission probability
- infectious_period
    - average period of an infectious individual
- simulate_day
    - integer
    - days to simulate
- latent_period
    - average period of an exposed individual
- prob_death
    - after state I, probability to be death
- prob_immute
    - after state I, probability to recover and immute
- vaccine_population
    - integer
    - vaccine population per day
- vaccine_efficiency
    - varies for all-or-nothing or leaky model

### Init Infector File

#### Format

```
n
node_id1 node_id2 node_id3 ...
```

#### Explanation

- `n` is the number of initial infectors, followed by `n` numbers indecate the ID of initial infectors
- this is supposed to be constructed by initalization modeule

### Vaccination Order File

#### Format

```
n
node_id1 node_id2 node_id3 ...
```

#### Explanation

- `n` is the number of nodes to be vaccine, followed by their ID
- `n` can be smaller than the number of all nodes
- in the simulation, it pick first `vaccine_population` number of nodes in the order list which is in the state S, and vaccine
    - if the top of the order is in other state, it just skip it
    - if the order list is ended, no vaccination after that