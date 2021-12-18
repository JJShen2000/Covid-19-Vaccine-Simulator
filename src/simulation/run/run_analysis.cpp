#include <iostream>
#include <fstream>

#include "../SimAonObjective.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 6) {
        std::cout << "arg error\n";
        return 1;
    }

    string p_graph = argv[1];
    string p_param = argv[2];
    string p_init = argv[3];
    string p_vacc = argv[4];

    ifstream in_graph(p_graph);
    ifstream in_param(p_param);
    ifstream in_init(p_init);
    ifstream in_vacc(p_vacc);

    string out_fname = argv[5];

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

    SimAonObjective model;
    model.load(in_graph, in_param, in_init, in_vacc);
    model.setOutFile(out_fname);

    model.simulate();

    return 0;
}