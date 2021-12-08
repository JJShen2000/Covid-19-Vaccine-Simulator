#include <iostream>
#include <fstream>

#include "../SFEIRVDSimulationLeaking.hpp"

using namespace std;



int main() {

    string p_graph = "./data/graph_10x10.txt";
    string p_param = "./data/param_SFEIRVD.txt";
    string p_init = "./data/init_SFEIRVD.txt";
    string p_vacc = "./data/vacc_SFEIRVD.txt";

    ifstream in_graph(p_graph);
    ifstream in_param(p_param);
    ifstream in_init(p_init);
    ifstream in_vacc(p_vacc);

    string out_fname = "./data/statistic_10x10_leaking.csv";

    if (!in_graph) {
        cout << "fail to open " << p_graph << '\n';
        return 0;
    }
    if (!in_param) {
        cout << "fail to open " << p_param << '\n';
        return 0;
    }
    if (!in_init) {
        cout << "fail to open " << p_init << '\n';
        return 0;
    }
    if (!in_vacc) {
        cout << "fail to open " << p_vacc << '\n';
        return 0;
    }

    SFEIRVDSimulationLeaking model;
    model.load(in_graph, in_param, in_init, in_vacc);
    model.setOutFile(out_fname);

    model.simulate();

    return 0;
}