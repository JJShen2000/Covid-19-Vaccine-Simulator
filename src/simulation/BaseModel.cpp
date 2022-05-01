#include "BaseModel.hpp"

void BaseModel::simulate() {
    for (auto& ts : tm) {
        simulate_unit(ts);
    }
}

void BaseModel::loadGraph(std::istream& in) {
    //std::cout << "load graph\n";
    in >> N_nd >> N_pr >> N_gp >> N_ag >> N_cm >> N_lc;
    tm.setPeriod(N_pr);
    cmp.resize(N_cm);
    cgpp.resize(N_gp);
    ndp.resize(N_nd);

    for (uint i = 0; i < N_gp; ++i) {
        uint j;
        in >> j;
        cgpp[i].setContactMatrix(ContactMatrix(&cmp[j]));
        cgpp[i].id = i;
        cgpp[i].nds.resize(N_ag);
    }

    for (uint i = 0; i < N_cm; ++i) {
        cmp[i].setSize(N_ag);
        double p;
        for (uint j = 0; j < N_ag; ++j) {
            for (uint k = 0; k < N_ag; ++k) {
                in >> p;
                cmp[i].setRate(j, k, p);
            }
        }
    }

    for (uint i = 0; i < N_nd; ++i) {
        auto& cur = ndp[i];
        in >> cur.age >> cur.loc;
        cur.id = i;
        cur.gp.resize(N_pr);
        for (uint j = 0; j < N_pr; ++j) {
            uint n;
            in >> n;
            for (uint k = 0; k < n; ++k) {
                uint gid;
                in >> gid;
                cur.gp[j].push_back(ContactGroup(&cgpp[gid]));
                cgpp[gid].nds.push_back(i);
                cgpp[gid].period = j;
                //cgpp[gid].nodes.push_back(Node(&cur));
            }
        }
    }
}

std::unordered_map<std::string, std::vector<double>> BaseModel::loadParam(std::istream& in) const {
    //std::cout << "load param\n";
    std::string str, vname;
    double val;
    std::unordered_map<std::string, std::vector<double>> mp;
    while (std::getline(in, str)) {
        std::stringstream ss;
        std::vector<double> vec;
        ss << str;
        ss >> vname;
        while (ss >> val) {
            vec.push_back(val);
        }
        mp[vname] = vec;
    }
    return mp;
}