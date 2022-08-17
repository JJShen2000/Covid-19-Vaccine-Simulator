#!/bin/bash

TERMINAL=$(tty)
sim_rounds=100
simulator="./simulator"
dbg=0

data_dir="../../data/sim_data/"
graph_dir="${data_dir}graph/*"
conf_dir="${data_dir}conf/*"
init_infector_dir="${data_dir}init_infectors/*"
vaccine_dir="${data_dir}vaccine_strat/*"

dump_dir="../../data/vis_data/"

preprocess()
{
    if ! [[ $1 =~ ^[0-9]+$ ]]; then
        echo $1 "ERROR: Invalid parameter"
        exit 1
    else
        sim_rounds=$1
    fi

	for arg in $@; do
		if [ $arg = "--debug" ]; then
			dbg=1
		fi
	done
}

makef()
{
    make

    wait
}

makec()
{
    make clean
}

run()
{
    gcnt=$(ls ${graph_dir} | wc -l)
    ccnt=$(ls ${conf_dir} | wc -l)
    icnt=$(ls ${init_infector_dir} | wc -l)
    vcnt=$(ls ${vaccine_dir} | wc -l)
    ttcnt=$((gcnt*ccnt*icnt*vcnt*sim_rounds))
    
    x=0
    start_time=$(date +%s.%3N)

    {
    for graph in ${graph_dir}; do
        for conf in ${conf_dir}; do
            for init_infectors in ${init_infector_dir}; do
                for vaccine in ${vaccine_dir}; do
                    gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    it="$(echo "$init_infectors" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    dd="$(echo "${dump_dir}${ct}_${it}_${vt}")"

                    for i in $(seq 1 $sim_rounds); do
                        pc=$((x*100/ttcnt))
                        
                        x=$((x+1))
                        rs="$(echo "${dd}/result_${i}.csv")"

                        if [ $i -eq 1 -a ! -d $dd ]; then 
                            mkdir $dd
                        fi
                        
                        end_time=$(date +%s.%3N)
                        elapsed=$(echo "scale=3; $end_time - $start_time" | bc)

                        msg1=$(printf "%-25s %25s\n" "Configurations" "${ct}")
                        msg2=$(printf "%-25s %25s\n" "Initial infectors" "${it}")
                        msg3=$(printf "%-25s %25s\n" "Vaccination strategy" "${vt}")
                        msg4=$(printf "%-25s %25s\n" "Rounds" "${i}")
                        msg5=$(printf "%-25s %25s\n" "Time Elapsed (seconds)" "${elapsed}")

                        wait
                        echo XXX
                        echo $pc
                        echo "$msg1"
                        echo "$msg2"
                        echo "$msg3"
                        echo "$msg4"
                        echo "$msg5"

                        echo XXX
                        ./${simulator} -g $graph -p $conf -i $init_infectors -v $vaccine -o $rs -s DSSASVWEIJKFRD >> time.log
                    done
                done
            done
        done
    done
    } | dialog --no-collapse --title "Simulating" --gauge "" 10 55 0
    clear
}

run_dbg_mode()
{
    gcnt=$(ls ${graph_dir} | wc -l)
    ccnt=$(ls ${conf_dir} | wc -l)
    icnt=$(ls ${init_infector_dir} | wc -l)
    vcnt=$(ls ${vaccine_dir} | wc -l)
    ttcnt=$((gcnt*ccnt*icnt*vcnt*sim_rounds))
    
    for graph in ${graph_dir}; do
        for conf in ${conf_dir}; do
            for init_infectors in ${init_infector_dir}; do
                for vaccine in ${vaccine_dir}; do
                    gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    it="$(echo "$init_infectors" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
                    dd="$(echo "${dump_dir}${ct}_${it}_${vt}")"

                    for i in $(seq 1 $sim_rounds); do
                        
                        if [ $i -eq 1 -a ! -d $dd ]; then 
                            mkdir $dd
                        fi
                        rs="$(echo "${dd}/result_${i}.csv")"

                        ./${simulator} -g $graph -p $conf -i $init_infectors -v $vaccine -o $rs -s DSSASVEIJKFRD
                        wait
                    done
                done
            done
        done
    done
}


main()
{
    preprocess $@

    makef
	if [ $dbg -eq 1 ]; then
		run_dbg_mode
	else
		run
	fi

    makec
}

main $@
