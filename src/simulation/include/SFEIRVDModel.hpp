#ifndef SFEIRVDMODEL_HPP
#define SFEIRVDMODEL_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <fstream>
#include <omp.h>

// #include "SusceptibleState.hpp"
#include "ExpiringState.hpp"
// #include "StableState.hpp"

#include "BaseModel.hpp"

class SFEIRVDModel : public BaseModel {
public:
    void load(std::istream& in_graph, std::istream& in_param, std::istream& in_init, std::istream& in_vacc) {
        std::cout << "Initializing...\n";
        loadGraph(in_graph);

        auto mp = loadParam(in_param);
        initParam(mp);

        icnt_all.resize(N_gp);
        icnt.resize(N_gp, std::vector<uint>(N_ag));

        loadVaccineOrder(in_vacc);

        auto init = loadInit(in_init);
        initInitInfector(init);
    }

    void simulate() {
        std::cout << "Simulating...\n";
        statisticInit();
        BaseModel::simulate();
        statisticEnd();
    }

protected:
    void initParam(std::unordered_map<std::string, std::vector<double>>& mp) {
        //std::cout << "init param\n";
        latent_period = mp["latent_period"][0];
        infectious_period = mp["infectious_period"][0];
        prob_transmission = mp["prob_transmission"];
        prob_death = mp["prob_death"];
        prob_immute = mp["prob_immute"];
        vaccine_population = mp["vaccine_population"][0];
        vaccine_efficiency = mp["vaccine_efficiency"][0];

        tm.setDay(mp["simulate_day"][0]);

        I.setAvgPeriod(infectious_period * tm.getPeriods());
        E.setAvgPeriod(latent_period * tm.getPeriods());
    }

    void loadVaccineOrder(std::istream& in_vacc) {
        //std::cout << "load vacc order\n";
        uint n, nid;
        in_vacc >> n;
        for (uint i = 0; i < n; ++i) {
            in_vacc >> nid;
            vacc_order.push_back(nid);
        }
    }

    std::vector<uint> loadInit(std::istream& in_init) {
        //std::cout << "load init\n";
        std::vector<uint> re;
        uint n = 0, nid;
        in_init >> n;
        for (uint i = 0; i < n; ++i) {
            in_init >> nid;
            re.push_back(nid);
        }
        return re;
    }

    void pushInfector(uint u) {
        //std::cout << "push " << u << '\n';
        I.insert(u);
        ndp[u].stateID = 'I';
        for (auto& cgps : ndp[u].gp) {
            for (auto& cgp : cgps) {
                ++icnt_all[cgp.getID()];
                ++icnt[cgp.getID()][ndp[u].age];
            } 
        }
    }

    void popInfector(uint u) {
        //std::cout << "pop " << u << '\n';
        for (auto& cgps : ndp[u].gp) {
            for (auto& cgp : cgps) {
                --icnt_all[cgp.getID()];
                --icnt[cgp.getID()][ndp[u].age];
            } 
        }
    }

    void initInitInfector(std::vector<uint>& inits) {
        //std::cout << "init init infec\n";
        std::sort(inits.begin(), inits.end());
        uint j = 0;
        for (uint i = 0; i < inits.size(); ++i) {
            while (j < inits[i]) {
                ndp[j].stateID = 'S';
                // S.insert(Node(&ndp[j]));
                ++j;
            }
            pushInfector(j);
            ++j;
        }
        while (j < N_nd) {
            ndp[j].stateID = 'S';
            // S.insert(Node(&ndp[j]));
            ++j;
        }
    }   

    inline std::vector<uint> infection(char src, char des, uint period, const std::vector<double>& ptrans) {
        std::vector<uint> re;
#pragma omp parallel num_threads(16)
        {
            std::vector<uint> tre;
#pragma omp for schedule(dynamic, 128) nowait
            for (uint i = 0; i < N_gp; ++i) {
                if (!icnt_all[i] || cgpp[i].period != period) continue;
                for (uint v = 0; v < N_ag; ++v) {
                    auto& aggp = cgpp[i].nds[v];
                    if (aggp.size() == 0) continue;

                    double m = 1;
                    for (uint u = 0; u < N_ag; ++u) {
                        m *= std::pow(1 - cgpp[i].cm.getRate(u, v) * ptrans[v], icnt[i][u]);
                    }
                    int k = Random::bino_dis(aggp.size(), 1 - m);
                    for (auto c : Random::choose(aggp.size(), k)) {
                        if (__sync_bool_compare_and_swap(&ndp[aggp[c]].stateID, src, des))
                            tre.push_back(aggp[c]);
                    }
                }
            }
            
#pragma omp critical
            {
                re.insert(re.end(), tre.begin(), tre.end());
            }
        }
        return re;
    }

    std::vector<uint> vaccination(char src) {
        static uint vacc_idx = 0;
        std::vector<uint> re;
        uint cnt = 0;
        while (cnt < vaccine_population && vacc_idx < vacc_order.size()) {
            uint cur = vacc_order[vacc_idx];
            if (ndp[cur].stateID == src) {
                re.push_back(cur);
                ++cnt;
            }
            ++vacc_idx;
        }
        return re;
    }

    void simulate_unit(const Time::TimeStep& ts) {
        //std::cout << "----simulate unit " << ts.getDay() << ' ' << ts.getPeriod() << "----\n";
        std::vector<uint> s2e = infection('S', 'E', ts.getPeriod(), prob_transmission); // chged
        std::vector<uint> f2e = infection('F', 'E', ts.getPeriod(), prob_transmission); // chged

        std::vector<uint> e2i = E.expire();
        std::vector<uint> i2frd = I.expire(), i2f, i2r, i2d;
        for (const auto& v : i2frd) {
            double pd = prob_death[ndp[v].age];
            if (Random::trail(pd)) {
                i2d.push_back(v);
            }
            else if (Random::trail(prob_immute[ndp[v].age] / (1 - pd))) {
                i2r.push_back(v);
            }
            else {
                i2f.push_back(v);
            }
        }

        for (auto v : s2e) E.insert(v);
        for (auto v : f2e) E.insert(v);
        for (auto v : e2i) pushInfector(v);


        for (auto v : i2f) {
            popInfector(v);
            ndp[v].stateID = 'F';
        }
        for (auto v : i2r) {
            popInfector(v);
            ndp[v].stateID = 'R';
        }
        for (auto v : i2d) {
            popInfector(v);
            ndp[v].stateID = 'D';
        }

        std::vector<uint> s2fv, s2f, s2v;
        if (ts.getPeriod() == 0) {
            s2fv = vaccination('S');
            for (const auto& v : s2fv) {
                if (Random::trail(vaccine_efficiency)) {
                    s2v.push_back(v);
                    ndp[v].stateID = 'V';
                }
                else {
                    s2f.push_back(v);
                    ndp[v].stateID = 'F';
                }
            }    
        }
        
        statisticUnit(ts, s2f, s2v, s2e, f2e, e2i, i2f, i2r, i2d);
    }

    // to override
    virtual void statisticInit() = 0;
    virtual void statisticUnit(const Time::TimeStep& ts, const std::vector<uint>& s2f, const std::vector<uint>& s2v, const std::vector<uint>& s2e, const std::vector<uint>& f2e, const std::vector<uint>& e2i, const std::vector<uint>& i2f, const std::vector<uint>& i2r, const std::vector<uint>& i2d) = 0;
    virtual void statisticEnd() = 0;

    //SuspictibleState S, F;
    ExpiringState E, I;
    //StableState R, V, D;
    std::vector<uint> icnt_all;
    std::vector<std::vector<uint>> icnt;

    std::vector<uint> vacc_order;

    uint vaccine_population;
    double vaccine_efficiency;
    double latent_period;
    double infectious_period;
    std::vector<double> prob_transmission;
    std::vector<double> prob_death;
    std::vector<double> prob_immute;
};

#endif