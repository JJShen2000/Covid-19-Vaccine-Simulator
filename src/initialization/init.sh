#!/bin/bash

if [[ ! -d "../../data/sim_data/group_tract_age/" || ! -f "../../data/sim_data/graph/graph.txt" ]]; then
    echo "Generating graph.txt"
    python3 initialization.py
fi