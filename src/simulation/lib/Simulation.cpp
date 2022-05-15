#include "Simulation.hpp"
#include <math.h>
#include <map>
#include <algorithm>
#define min(x, y) (((x) < (y))? (x) : (y))
// #define TEST_TIME
#ifdef TEST_TIME
#include <sys/time.h>
#endif

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

    epsilon_V1_bar = mp["epsilon_V1"];
    for (auto& v : epsilon_V1_bar) {
        v = 1 - v;
    }
    epsilon_V2_bar = mp["epsilon_V2"];
    for (auto& v : epsilon_V2_bar) {
        v = 1 - v;
    }

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
    score.resize(N_nd);

    std::string strat_name;
    getline(in, strat_name);
    if (strat_name == "infect_prob") {

    }

    std::map<std::string, uint> mp;
    std::string pname;
    uint pvalue;
    while (in >> pname >> pvalue) {
        mp[pname] = pvalue;
    }

    vacc_rollout = mp["rollout"];
    vacc_start_day = mp["first_dose_date"] - 1;
    vacc_sec_start_day = mp["second_dose_date"] - 1;
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
    switch (state) {
        case 'S':
            return 1;
        case 'V':
            return epsilon_V1_bar[time >> 1];
        case 'W':
            return epsilon_V2_bar[time >> 1];
    }
    return 1;
}

double Simulation::infect_prob(uint u, uint v, const ContactGroup& cgp, double tau, const Time::TimeStep& ts) {
    return epsilon_bar(ndp[v].stateID, ts - ndp[v].ts) * cgp.getContactMatrix().getRate(ndp[u].age, ndp[v].age) * tau;
}

void Simulation::infection(ExpiringState& ext, double tau, const Time::TimeStep& ts, Nodes& s2e, Nodes& v2e, Nodes& w2e, Nodes& f2e) {
    // cout << "one infection\n";
    // cout << "ext length " << ext.size() << '\n';
    for (auto& vec : ext) {
        #pragma omp parallel num_threads(8)
        {
            std::vector<uint> tre_s2e, tre_v2e, tre_w2e, tre_f2e;

            #pragma omp for schedule(dynamic)
            // for (auto u : vec) {
            for (long unsigned int i = 0; i < vec.size(); ++i) {
            // cout << "infect from " << u << '\n';
                for (auto& cgp: ndp[vec[i]].gp[ts.getPeriod()]) {
                // for (long unsigned int j = 0; j < ndp[vec[i]].gp[ts.getPeriod()].size(); ++j) {
                //     auto& cgp = ndp[vec[i]].gp[ts.getPeriod()][j];
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
                        // uint dt = ts - ndp[v].ts;
                        // cout << "got time\n";
                        double p = infect_prob(vec[i], v, cgp, tau, ts);
                        // double p = epsilon_bar(ndp[v].stateID, dt) * cgp.getContactMatrix().getRate(ndp[vec[i]].age, ndp[v].age) * tau;
                        if (Random::trial(p * n / k)) {
                            // switch (ndp[v].stateID) {
                            //     case 'S':
                            //         s2e.push_back(v);
                            //         ndp[v].stateID = 'E';
                            //         break;
                            //     case 'V':
                            //         v2e.push_back(v);
                            //         ndp[v].stateID = 'E';
                            //         break;
                            //     case 'W':
                            //         w2e.push_back(v);
                            //         ndp[v].stateID = 'E';
                            //         break;
                            //     case 'F':
                            //         f2e.push_back(v);
                            //         ndp[v].stateID = 'E';
                            //         break;
                            // }
                            
                            if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'S', 'E'))
                                tre_s2e.push_back(v);
                            else if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'V', 'E'))
                                tre_v2e.push_back(v);
                            else if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'W', 'E'))
                                tre_w2e.push_back(v);
                            else if (__sync_bool_compare_and_swap(&ndp[v].stateID, 'F', 'E'))
                                tre_f2e.push_back(v);
                        }
                    }
                }
            }

            # pragma omp critical
            {
                s2e.insert(s2e.end(), tre_s2e.begin(), tre_s2e.end());
                v2e.insert(v2e.end(), tre_v2e.begin(), tre_v2e.end());
                w2e.insert(w2e.end(), tre_w2e.begin(), tre_w2e.end());
                f2e.insert(f2e.end(), tre_f2e.begin(), tre_f2e.end());
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

void Simulation::vaccination(Nodes& s2v, Nodes& v2w, const Time::TimeStep& ts) {
    // uint success_cnt = 0;
    // while (success_cnt < vacc_rollout && !vacc_queue.empty()) {
    //     uint u = vacc_queue.top();
    //     vacc_queue.pop();
    //     switch (ndp[u].stateID) {
    //         case 'S':
    //             s2v.push_back(u);
    //             ++success_cnt;
    //             break;
    //         case 'V':
    //             v2w.push_back(u);
    //             ++success_cnt;
    //             break;
    //     }
    // }
    
    // return std::vector<uint>();
    std::vector<std::pair<double, uint>> order;
    for (uint i = 0; i < N_nd; ++i) {
        char st = ndp[i].stateID;
        if (st == 'S' || (ts.getDay() >= vacc_sec_start_day && st == 'V')) {
            order.push_back({score[i], i});
        }
    }
    sort(order.begin(), order.end());
    // cout << "order\n";
    // for (auto& v : order) {
    //     cout << v.second << '\t' << v.first << '\n';
    // }
    // for (uint i = 0; i < order.size(); ++i) {
    //     if (ndp[order[i].second].stateID == 'V') {
    //         cout << "i-th " << i << '\n';
    //         break;
    //     }
    // }

    for (uint i = 0; i < min(order.size(), vacc_rollout); ++i) {
        // cout << "i " << i << '\n';
        uint u = order[i].second;
        if (ndp[u].stateID == 'S') {
            s2v.push_back(u);
        }
        else {
            // cout << "v2w! " << u << '\n';
            v2w.push_back(u);
        }
    }
}

void Simulation::increaseScore(uint u, double tau, const Time::TimeStep& ts) {
    for (auto& tcgp : ndp[u].gp) {
        for (auto& cgp : tcgp) {
            for (uint i = 0; i < cgp.size(); ++i) {
                uint v = cgp.at(i);
                char st = ndp[v].stateID;
                if (st == 'S' || (st == 'V' && ts.getDay() >= vacc_sec_start_day)) {
                    double p = infect_prob(u, v, cgp, tau, ts);
                    score[v] += log2(1 - p);
                } 
                
            }
        }
    }
}

void Simulation::updateScore(const Time::TimeStep& ts) {
    for (uint i = 0; i < N_nd; ++i) score[i] = 0.0;

    for (auto& vec : I_pre) {
        for (uint i = 0; i < vec.size(); ++i) {
            increaseScore(vec[i], tau_I_pre, ts);
        }
    }

    for (auto& vec : I_asym) {
        for (uint i = 0; i < vec.size(); ++i) {
            increaseScore(vec[i], tau_I_asym, ts);
        }
    }

    for (auto& vec : I_sym) {
        for (uint i = 0; i < vec.size(); ++i) {
            increaseScore(vec[i], tau_I_sym, ts);
        }
    }
}

void Simulation::simulate_unit(const Time::TimeStep& ts) {
    cout << "------- " << ts.getDay() << ' ' << ts.getPeriod() << "-------" << '\n';
    Transition trans;
#ifdef TEST_TIME
    timeval st, ed;
    gettimeofday(&st, 0);
#endif

    if (ts.getDay() >= vacc_start_day && ts.getPeriod() == 0) {
        // update score
        cout << "calculate score\n";
        updateScore(ts);

        // vaccination
        cout << "vaccination\n";
#ifdef TEST_TIME
        gettimeofday(&st, 0);
#endif
        vaccination(trans.s2v, trans.v2w, ts);
#ifdef TEST_TIME
        gettimeofday(&ed, 0);
        cout << "\tTime: "<< ed.tv_sec - st.tv_sec + (ed.tv_usec - st.tv_usec) / 1000000.0 << " sec\n";
#endif
        for (auto v : trans.s2v) {
            ndp[v].stateID = 'V';
            ndp[v].ts = ts;
        }
        for (auto v : trans.v2w) {
            ndp[v].stateID = 'W';
            ndp[v].ts = ts;
        }

    }

    // extract
    cout << "infect\n";
    infection(I_pre, tau_I_pre, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);
    infection(I_asym, tau_I_asym, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);
    infection(I_sym, tau_I_sym, ts, trans.s2e, trans.v2e, trans.w2e, trans.f2e);
#ifdef TEST_TIME
    gettimeofday(&ed, 0);
    cout << "\tTime: "<< ed.tv_sec - st.tv_sec + (ed.tv_usec - st.tv_usec) / 1000000.0 << " sec\n";
#endif
    partitionGroup(I_pre.expire(), trans.i2j, trans.i2k, sigma_asym);

    Nodes sym_not_d;
    cout << "expire\n";
#ifdef TEST_TIME
    gettimeofday(&st, 0);
#endif
    partitionGroupAge(I_sym.expire(), trans.k2d, sym_not_d, prob_death_sym);
    partitionGroupAge(sym_not_d, trans.k2r, trans.k2f, prob_immune_sym_cond_nd);

    partitionGroupAge(I_asym.expire(), trans.j2r, trans.j2f, prob_immune_asym);

    trans.e2i = E.expire();
#ifdef TEST_TIME
    gettimeofday(&ed, 0);
    cout << "\tTime: "<< ed.tv_sec - st.tv_sec + (ed.tv_usec - st.tv_usec) / 1000000.0 << " sec\n";
#endif
        
    
    // insert
    cout << "update state\n";
#ifdef TEST_TIME
    gettimeofday(&st, 0);
#endif
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

#ifdef TEST_TIME
    gettimeofday(&ed, 0);
    cout << "\tTime: "<< ed.tv_sec - st.tv_sec + (ed.tv_usec - st.tv_usec) / 1000000.0 << " sec\n";
#endif

#ifdef TEST_TIME
    gettimeofday(&st, 0);
#endif
    statisticUnit(ts, trans);
#ifdef TEST_TIME
    gettimeofday(&ed, 0);
    cout << "\tTime: "<< ed.tv_sec - st.tv_sec + (ed.tv_usec - st.tv_usec) / 1000000.0 << " sec\n";
#endif
}
