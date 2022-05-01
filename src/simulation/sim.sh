#!/bin/bash

no_aon=0
no_leaky=0

simulator="./simulator"

data_dir="../../data/sim_data/"
graph_dir="${data_dir}graph/*"
conf_dir="${data_dir}conf/*"
init_infector_dir="${data_dir}init_infector/*"
vaccine_dir="${data_dir}vaccine/*"

dump_dir="../../data/vis_data/"

preprocess()
{
	for arg in $@; do
        case $arg in
            "--without-aon")
                no_aon=1   
                ;;
            "--without-leaky")
                no_leaky=1
                ;;
        esac
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
	if [ $no_leaky -eq 1 ]; then
		return
	fi

	for graph in ${graph_dir}; do
		for conf in ${conf_dir}; do
			for init_infector in ${init_infector_dir}; do
				for vaccine in ${vaccine_dir}; do
					gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					it="$(echo "$init_infector" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					rs="$(echo "${dump_dir}${gt}_${ct}_${it}_${vt}_result.csv")"

					echo "simulating: " $gt $ct $it $vt
					echo "          dest file: " $rs

					./${simulator} -g $graph -p $conf -i $init_infector -v $vaccine -o $rs -s DESSIR
					wait
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
