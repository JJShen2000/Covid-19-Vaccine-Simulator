#!/bin/bash

sim_rounds=100

preprocess()
{
    for arg in $@; do
        case $arg in
            "-C")
                rm -rf data
                exit 0
                ;;
            "-c")
                rm -rf data/vis_data/*
                exit 0
                ;;
            "-d")
                vis
                exit 0
                ;;
            *)
                if ! [[ $arg =~ ^[0-9]+$ ]]; then
                    echo $arg "ERROR: Invalid parameter"
                    exit 1
                else
                    sim_rounds=$arg
                fi
        esac
    done
}

data_process()
{
    echo "Running Data Processing..."

    cd src/data_process/
    source data_process.sh
    cd ../..

    wait
}

init()
{
    echo "Running Initialization..."

    cd src/initialization/
    source init.sh
    cd ../..

    wait
}

sim()
{
    echo "Running Simulation..."
    cd src/simulation
    source sim.sh $sim_rounds
    cd ../..

    wait
}

vis()
{
    echo "Running Visualization..."

    cd src/visualization
    python3 draw_state.py
    cd ../..

    wait
}

main()
{
    preprocess $@

    data_process
    init
    sim
    vis
}

main $@
