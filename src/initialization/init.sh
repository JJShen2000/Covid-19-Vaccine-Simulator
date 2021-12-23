#!/bin/bash

if [[ ! -d "../../data/sim_data/group_tract_age/" || ! -f "../../data/sim_data/graph/graph.txt" ]]; then
    echo "Generating graph.txt"
    python3 initialization.py
fi

echo "Generating init infector doc"
python3 init_infectors.py

echo "Generating vaccination docs"
python3 vaccine_order.py
