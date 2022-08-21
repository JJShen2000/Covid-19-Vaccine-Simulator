root="../.."
sim_data="$root/data/sim_data"
simulation_path="$root/src/simulation"

conf_path="./basic.conf"
init_path="$sim_data/init_infectors/wanhua.csv"
graph_path="$sim_data/graph/graph.txt"

vacc_prfx="./vacc_strat_infect_prob"
vacc_temp="${vacc_prfx}_template.conf"
res_prfx="./"

../../src/initialization/init.sh

cd $simulation_path
make
cd ../../test/spread_speed

for i in $(seq 1 10 51);
do
    vacc_path="$vacc_prfx_$i.conf"
    res_path="$(echo "${res_prfx}res_$i.csv")"

    cat $vacc_temp > $vacc_path
    sed -i "s/abc/$i/g" $vacc_path

    wait

    echo $i
    # ls
    "${simulation_path}/simulator" -g $graph_path -p $conf_path -i $init_path -v $vacc_path -o $res_path -s DSSASVWEIJKFRD
done