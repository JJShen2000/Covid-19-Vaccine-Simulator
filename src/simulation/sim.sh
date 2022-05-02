#!/bin/bash

no_aon=0
no_leaky=0

simulator="./simulator"

data_dir="../../data/sim_data/"
graph_dir="${data_dir}graph/*"
conf_dir="${data_dir}conf/*"
init_infector_dir="${data_dir}init_infectors/*"
vaccine_dir="${data_dir}vaccine_strat/*"

dump_dir="../../data/vis_data/"

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
					rs="$(echo "${dump_dir}${gt}__${ct}__${it}__${vt}__result.csv")"

					#echo "simulating: " $gt $ct $it $vt
					echo "simulating: " $graph $conf $init_infectors $vaccine
					#echo "dest file: " $rs

					./${simulator} -g $graph -p $conf -i $init_infectors -v $vaccine -o $rs -s DESSIR
					wait
				done
			done
		done
	done
}

main()
{
	makef

	run

	makec
}

main $@
