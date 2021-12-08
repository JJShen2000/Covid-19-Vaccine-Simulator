#!/bin/bash

no_dp=0
no_init=0
no_sim=0
no_vis=0

preprocess()
{
    for arg in $@; do
        case $arg in
            "--without-dp")
                no_dp=1   
                ;;
            "--without-init")
                no_init=1
                ;;
            "--without-sim")
                no_sim=1
                ;;
            "--without-vis")
                no_vis=1
                ;;
        esac
    done
}

data_process()
{
    if [ $no_dp -eq 1 ]; then
        return
    fi

    echo "Running Data Processing..."

    cd src/data_process/
    source data_process.sh
    cd ../..

    wait
}

init()
{
    if [ $no_init -eq 1 ]; then
        return
    fi

    echo "Running Initialization..."

    cd src/initialization/
    source init.sh
    cd ../..

    wait
}

sim()
{
    if [ $no_sim -eq 1 ]; then
        return
    fi

    echo "Running Simulation..."
    
    cd src/simulation
    source sim.sh
    cd ../..

    wait
}

vis()
{
    if [ $no_vis -eq 1 ]; then
        return
    fi

    echo "Running Visualization..."

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
