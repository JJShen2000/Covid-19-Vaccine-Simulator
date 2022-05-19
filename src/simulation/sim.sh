#!/bin/bash

sim_rounds=100
simulator="./simulator"

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
	for graph in ${graph_dir}; do
		for conf in ${conf_dir}; do
			for init_infectors in ${init_infector_dir}; do
				for vaccine in ${vaccine_dir}; do
					gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
					ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
					it="$(echo "$init_infectors" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
					vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//; s/.csv//')"
					dd="$(echo "${dump_dir}${ct}_${it}_${vt}")"

					#echo "simulating: " $graph $conf $init_infectors $vaccine
					echo "simulating: " $gt $ct $it $vt

					for i in $(seq 1 $sim_rounds); do
						rs="$(echo "${dd}/result_${i}.csv")"

						if [ $i -eq 1 ]; then 
							mkdir $dd
						fi

						./${simulator} -g $graph -p $conf -i $init_infectors -v $vaccine -o $rs -s DSSASVWEIJKFRD
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
	run
	makec
}

main $@
