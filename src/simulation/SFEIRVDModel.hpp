#ifndef SFEIRVDMODEL_HPP
#define SFEIRVDMODEL_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
//#include <algorithm>
#include <string>
#include <fstream>

#include "SusceptibleState.hpp"
#include "ExpiringState.hpp"
#include "StableState.hpp"

#include "BaseModel.hpp"

class SFEIRVDModel : public BaseModel {
public:
    void load(std::istream& in_graph, std::istream& in_param, std::istream& in_init, std::istream& in_vacc) {
        std::cout << "Initializing...\n";
        loadGraph(in_graph);

        auto mp = loadParam(in_param);
        initParam(mp);

        S.init(N_gp, N_ag, cgpp);
        F.init(N_gp, N_ag, cgpp);

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

    void initInitInfector(std::vector<uint>& inits) {
        //std::cout << "init init infec\n";
        std::sort(inits.begin(), inits.end());
        uint j = 0;
        for (uint i = 0; i < inits.size(); ++i) {
            while (j < inits[i]) {
                ndp[j].stateID = 'S';
                S.insert(Node(&ndp[j]));
                ++j;
            }
            ndp[j].stateID = 'I';
            I.insert(Node(&ndp[j]));
            ++j;
        }
        while (j < N_nd) {
            ndp[j].stateID = 'S';
            S.insert(Node(&ndp[j]));
            ++j;
        }
    }   

    inline Nodes infection(SuspictibleState& Sus, uint period) {
        // Nodes sus2i;
        // for (uint i = 0; i < I.size(); ++i) {
        //     Nodes tmp = Sus.infected(I[i], period, prob_transmission);
        //     sus2i.insert(sus2i.end(), tmp.begin(), tmp.end());
        // }
        // return sus2i;
        
        std::unordered_map<uint, std::vector<uint>> icnt; // contact group id
        for (uint i = 0; i < I.size(); ++i) {
            auto u = I[i];
            for (auto& cgp : u.getGroups()[period]) {
                uint gid = cgp.getID();
                if (!icnt.count(gid)) {
                    icnt[gid].resize(N_ag);
                }
                ++icnt[gid][u.getAge()];
            }
        }

        std::unordered_set<uint> sus2i; // node id
        for (auto& p : icnt) {
            Sus[p.first].infected(p.second, prob_transmission, sus2i);
        }

        Nodes re;
        for (auto v : sus2i) {
            Sus.erase(Node(&ndp[v]));
            re.push_back(Node(&ndp[v]));
        }
        return re;
    }

    Nodes vaccination() {
        //std::cout << "vaccination\n";
        static uint vacc_idx = 0;
        Nodes re;
        uint cnt = 0;
        while (cnt < vaccine_population && vacc_idx < vacc_order.size()) {
            uint cur = vacc_order[vacc_idx];
            //std::cout << "try vacc " << cur.getID() << '\n';
            // if (cur.getState() == 'S') {
            //     //std::cout << "vacc " << cur.getID() << '\n';
            //     S.erase(cur);
            //     re.push_back(cur);
            //     ++cnt;
            // }
            if (ndp[cur].stateID == 'S') {
                Node nd(&ndp[cur]);
                S.erase(nd);
                re.push_back(nd);
                ++cnt;
            }
            ++vacc_idx;
        }
        return re;
    }

    void simulate_unit(const Time::TimeStep& ts) {
        //std::cout << "----simulate unit " << ts.getDay() << ' ' << ts.getPeriod() << "----\n";
        Nodes s2e = infection(S, ts.getPeriod());
        Nodes f2e = infection(F, ts.getPeriod());

        Nodes e2i = E.expire();
        Nodes i2frd = I.expire(), i2f, i2r, i2d;
        for (const auto& v : i2frd) {
            double pd = prob_death[v.getAge()];
            if (Random::trail(pd)) {
                i2d.push_back(v);
            }
            else if (Random::trail(prob_immute[v.getAge()] / (1 - pd))) {
                i2r.push_back(v);
            }
            else {
                i2f.push_back(v);
            }
        }

        s2e.setState('E');
        E.insert(s2e);

        f2e.setState('E');
        E.insert(f2e);

        e2i.setState('I');
        I.insert(e2i);

        i2f.setState('F');
        F.insert(i2f);

        i2r.setState('R');
        R.insert(i2r);

        i2d.setState('D');
        D.insert(i2d);

        Nodes s2fv = vaccination(), s2f, s2v;
        for (const auto& v : s2fv) {
            if (Random::trail(vaccine_efficiency)) {
                s2v.push_back(v);
            }
            else {
                s2f.push_back(v);
            }
        }

        statisticUnit(ts, s2f, s2v, s2e, f2e, e2i, i2f, i2r, i2d);

        s2f.setState('F');
        F.insert(s2f);

        s2v.setState('V');
        V.insert(s2v);
    }

    // to override
    virtual void statisticInit() = 0;
    virtual void statisticUnit(const Time::TimeStep& ts, const Nodes& s2f, const Nodes& s2v, const Nodes& s2e, const Nodes& f2e, const Nodes& e2i, const Nodes& i2f, const Nodes& i2r, const Nodes& i2d) = 0;
    virtual void statisticEnd() = 0;

    SuspictibleState S, F;
    ExpiringState E, I;
    StableState R, V, D;

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