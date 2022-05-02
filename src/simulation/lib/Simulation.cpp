#include "Simulation.hpp"

#include <iostream>
void Simulation::loadGraph(std::istream& in) {
    BaseModel::loadGraph(in);
}

void Simulation::loadParam(std::istream& in) {
    auto mp = BaseModel::loadParam(in);
    // std::cout << "get param\n";
    // for (auto p : mp) {
    //     cout << p.first << ' ' << p.second.size() << '\n';
    // }

    tm.setDay(mp["simulate_day"][0]);
    for (uint i = 0; i < N_nd; ++i) {
        ndp[i].ts = tm.begin();
    }

    gamma_E = mp["gamma_E"][0];
    gamma_I_pre = mp["gamma_I_pre"][0];
    gamma_I_asym = mp["gamma_I_asym"][0];
    gamma_I_sym = mp["gamma_I_sym"][0];

    E.setAvgPeriod(gamma_E * tm.getPeriods());
    I_pre.setAvgPeriod(gamma_I_pre * tm.getPeriods());
    I_asym.setAvgPeriod(gamma_I_asym * tm.getPeriods());
    I_sym.setAvgPeriod(gamma_I_sym * tm.getPeriods());

    // cout << "set period ok\n";

    tau_I_pre = mp["tau_I_pre"][0];
    tau_I_asym = mp["tau_I_asym"][0];
    tau_I_sym = mp["tau_I_sym"][0];

    sigma_asym = mp["sigma_I_asym"][0];

    epsilon_V1 = mp["epsilon_V1"];
    epsilon_V2 = mp["epsilon_V2"];

    // cout << "ms1\n";

    prob_trans = mp["prob_transmission"];
    for (uint k = 0; k < N_cm; ++k) {
        for (uint i = 0; i < N_ag; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                cmp[k].cm[i][j] *= prob_trans[j];
                // cout << k << ' ' << i << ' ' << j << ' ' << cmp[k].cm[i][j] << '\n';
            }
        }
        cmp[k].computePmax();
    }

    prob_death_sym = mp["prob_death_sym"];

    prob_immune_asym = mp["prob_immune_asym"];
    prob_immune_sym = mp["prob_immune_sym"];
    // cout << "get all\n";
    for (uint i = 0; i < N_ag; ++i) {
        if (prob_death_sym[i] == 1) {
            prob_immune_sym_cond_nd.push_back(0);
        }
        else
            prob_immune_sym_cond_nd.push_back(prob_immune_sym[i] / (1 - prob_death_sym[i]));
    }
}

void Simulation::loadVaccinationStrat(std::istream& in) {

}

void Simulation::loadInitInfector(std::istream& in) {
    std::vector<std::vector<uint>> coll(N_lc);
    for (uint i = 0; i < N_nd; ++i) {
        coll[ndp[i].loc].push_back(ndp[i].id);
        ndp[i].stateID = 'S';
    }

    // cout << "list:\n";
    // for (uint i = 0; i < N_lc; ++i) {
    //     for (auto v : coll[i]) {
    //         cout << v << ' ';
    //     }
    //     cout << '\n';
    // }

    uint n;
    in >> n;
    for (uint i = 0; i < n; ++i) {
        Location loc;
        uint num;
        in >> loc >> num;
        auto chosen = Random::choose(coll[loc].size(), num);
        for (auto v : chosen) {
            ndp[coll[loc][v]].stateID = 'I';
            I_pre.insert(coll[loc][v]);
        }
    }
}

void Simulation::simulate() {
    statisticInit();
    BaseModel::simulate();
    statisticEnd();
}

// vaccine ineffectiveness
double Simulation::epsilon_bar(char state, uint time) {
    return 1;
}

void Simulation::infection(ExpiringState& ext, double tau, const Time::TimeStep& ts, Nodes& s2e, Nodes& v2e, Nodes& w2e, Nodes& f2e) {
    // cout << "one infection\n";
    // cout << "ext length " << ext.size() << '\n';
    for (auto& vec : ext) {
        for (auto u : vec) {
            // cout << "infect from " << u << '\n';
            for (auto& cgp : ndp[u].gp[ts.getPeriod()]) {
                /*
                std::vector<uint> re_s2e, re_v2e, re_w2e;
                */
                
                
                uint n = cgp.size();
                double k = ceil(n * cgp.getContactMatrix().getPmax());
                // cout << "try group " << cgp.getID() << ' ' << cgp.size() << ' ' << k << ' ' << cgp.getContactMatrix().getPmax() << '\n';
                for (auto idx : Random::choose(n, k)) {
                    // cout << "chosen " << k << " from " << n << '\n';
                    uint v = cgp.at(idx);
                    // cout << "get time\n";
                    uint dt = ts - ndp[v].ts;
                    // cout << "got time\n";
                    double p = epsilon_bar(ndp[v].stateID, dt) * cgp.getContactMatrix().getRate(ndp[u].age, ndp[v].age) * tau;
                    if (Random::trial(p * n / k)) {
                        //
                        switch (ndp[v].stateID) {
                            case 'S':
                                s2e.push_back(v);
                                ndp[v].stateID = 'E';
                                break;
                            case 'V':
                                v2e.push_back(v);
                                ndp[v].stateID = 'E';
                                break;
                            case 'W':
                                w2e.push_back(v);
                                ndp[v].stateID = 'E';
                                break;
                            case 'F':
                                f2e.push_back(v);
                                ndp[v].stateID = 'E';
                                break;
                        }
                        //

                        /*
                        if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'S', 'E'))
                            tre_s2e.push_back(aggp[c]);
                        else if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'V', 'E'))
                            tre_v2e.push_back(aggp[c]);
                        else if (__sync_bool_compare_and_swap(&ndp[aggp[c]].stateID, 'W', 'E'))
                            tre_w2e.push_back(aggp[c]);
                        else if (__sync_bool_compare_and_swap(&ndp[aggp[c]].stateID, 'F', 'E'))
                            tre_f2e.push_back(aggp[c]);
                        */
                    }
                }

                /*
                s2e.insert(s2e.end(), tre_s2e.begin(), tre_s2e.end());
                v2e.insert(v2e.end(), tre_v2e.begin(), tre_v2e.end());
                w2e.insert(w2e.end(), tre_w2e.begin(), tre_w2e.end());
                */
            }
        }
    }
}

void Simulation::partitionGroup(const Nodes& vorg, Nodes& v1, Nodes& v2, double p1) {
    for (auto v : vorg) {
        if (Random::trial(p1)) v1.push_back(v);
        else v2.push_back(v);
    }
}

void Simulation::partitionGroupAge(const Nodes& vorg, Nodes& v1, Nodes& v2, const std::vector<double>& p1) {
    for (auto v : vorg) {
        if (Random::trial(p1[ndp[v].age])) v1.push_back(v);
        else v2.push_back(v);
    }
}

std::vector<uint> Simulation::vaccination(Nodes& s2v, Nodes& v2w) {
    uint success_cnt = 0;
    while (success_cnt < vacc_rollout && !vacc_queue.empty()) {
        uint u = vacc_queue.top();
        vacc_queue.pop();
        switch (ndp[u].stateID) {
            case 'S':
                s2v.push_back(u);
                ++success_cnt;
                break;
            case 'V':
                v2w.push_back(u);
                ++success_cnt;
                break;
        }
    }
    
    return std::vector<uint>();
}

void Simulation::simulate_unit(const Time::TimeStep& ts) {
    cout << "------- " << ts.getDay() << ' ' << ts.getPeriod() << "-------" << '\n';
    Transition trans;
    // extract
    cout << "infect\n";
    infection(I_pre, tau_I_pre, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);
    infection(I_asym, tau_I_asym, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);
    infection(I_sym, tau_I_sym, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);

    partitionGroup(I_pre.expire(), trans.i2j, trans.i2k, sigma_asym);

    Nodes sym_not_d;
    cout << "expire\n";
    partitionGroupAge(I_sym.expire(), trans.k2d, sym_not_d, prob_death_sym);
    partitionGroupAge(sym_not_d, trans.k2r, trans.k2f, prob_immune_sym);

    partitionGroupAge(I_asym.expire(), trans.j2r, trans.j2f, prob_immune_sym_cond_nd);

    trans.e2i = E.expire();

    // vaccination
    if (ts.getPeriod() == 0) {
        cout << "vaccination\n";
        vaccination(trans.s2v, trans.v2w);
    }
        
    
    // insert
    cout << "update state\n";
    for (auto v : trans.s2e) E.insert(v);
    for (auto v : trans.v2e) E.insert(v);
    for (auto v : trans.w2e) E.insert(v);
    for (auto v : trans.f2e) E.insert(v);

    // cout << "insert\n";
    for (auto v : trans.e2i) {
        ndp[v].stateID = 'I';
        I_pre.insert(v);
    }
    for (auto v : trans.i2j) {
        ndp[v].stateID = 'J';
        I_asym.insert(v);
    }
    for (auto v : trans.i2k) {
        ndp[v].stateID = 'K';
        I_sym.insert(v);
    }

    for (auto v : trans.k2d) ndp[v].stateID = 'D';
    for (auto v : trans.k2r) ndp[v].stateID = 'R';
    for (auto v : trans.k2r) ndp[v].stateID = 'F';
    for (auto v : trans.j2r) ndp[v].stateID = 'R';
    for (auto v : trans.j2f) ndp[v].stateID = 'F';

    for (auto v : trans.s2v) {
        ndp[v].stateID = 'V';
        ndp[v].ts = ts;
    }
    for (auto v : trans.v2w) {
        ndp[v].stateID = 'W';
        ndp[v].ts = ts;
    }

    // update score
    cout << "update score\n";

    statisticUnit(ts, trans);
}
