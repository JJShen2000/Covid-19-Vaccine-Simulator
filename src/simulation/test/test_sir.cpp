#include <fstream>

#include "../Debug.hpp"

#include "../SIRModel.hpp"

int main() {

    SIRModel model;

    std::ifstream fin_graph("./data/graph_10x10.txt");
    std::ifstream fin_param("./data/param_sir.conf");
    std::ifstream fin_init("./data/init_sir.conf");

#ifdef DEBUG
    if (!fin_graph || !fin_param || !fin_init) {
        LOG("[ERROR] load file error")
    }
#endif

    model.load(fin_graph, fin_param, fin_init);

    model.simulate();

    return 0;
}