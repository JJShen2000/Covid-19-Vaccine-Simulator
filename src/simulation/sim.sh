#!/bin/bash

no_aon=0
no_leaky=0

aon_fn="sfeirvd_aon"
leaky_fn="sfeirvd_leaky"

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

leaky()
{
	if [ $no_leaky -eq 1 ]; then
		return
	fi

	echo "simulating leaky model"

	for graph in ${graph_dir}; do
		for conf in ${conf_dir}; do
			for init_infector in ${init_infector_dir}; do
				for vaccine in ${vaccine_dir}; do
					gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					it="$(echo "$init_infector" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					dump_fn="$(echo "${dump_dir}${leaky_fn}_${gt}_${ct}_${it}_${vt}.csv")"

					./${leaky_fn} $graph $conf $init_infector $vaccine $dump_fn
					wait
				done
			done
		done
	done

}

all_or_nothing()
{
	if [ $no_aon -eq 1 ]; then
		return
	fi

	echo "simulating all-or-nothing model"

	for graph in ${graph_dir}; do
		for conf in ${conf_dir}; do
			for init_infector in ${init_infector_dir}; do
				for vaccine in ${vaccine_dir}; do
					gt="$(echo "$graph" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					ct="$(echo "$conf" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					it="$(echo "$init_infector" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					vt="$(echo "$vaccine" | awk -F / '{print $NF}' | sed 's/.txt//; s/.conf//')"
					dump_fn="$(echo "${dump_dir}${aon_fn}_${gt}_${ct}_${it}_${vt}.csv")"

					./${aon_fn} $graph $conf $init_infector $vaccine $dump_fn
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

	leaky
	all_or_nothing

	makec
}

main $@
