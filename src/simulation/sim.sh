#!/bin/bash

no_aon=0
no_leaky=0

data_dir=""

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

leaky()
{
	if [ $no_leaky -eq 1 ]; then
		return
	fi

	echo "simulating leaky model"

	
}

all_or_nothing()
{
	if [ $no_aon -eq 1 ]; then
		return
	fi

	echo "simulating all-or-nothing model"

}

main()
{
	preprocess $@

}

main $@