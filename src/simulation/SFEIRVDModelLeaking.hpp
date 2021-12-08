#ifndef SFEIRVDMODELLEAKING_HPP
#define SFEIRVDMODELLEAKING_HPP

#include <iostream>
#include <unordered_map>
//#include <algorithm>
#include <string>
#include <fstream>

#include "SuspictibleState.hpp"
#include "ExpiringState.hpp"
#include "StableState.hpp"

#include "BaseModel.hpp"

class SFEIRVDModelLeaking : public BaseModel {
public:
    void load(std::istream& in_graph, std::istream& in_param, std::istream& in_init, std::istream& in_vacc) {
        //std::cout << "load\n";
        loadGraph(in_graph);

        auto mp = loadParam(in_param);
        initParam(mp);

        S.init(N_gp, N_ag, cgpp);
        F.init(N_gp, N_ag, cgpp);
        V.init(N_gp, N_ag, cgpp);

        loadVaccineOrder(in_vacc);

        auto init = loadInit(in_init);
        initInitInfector(init);
    }

    void simulate() {
        //std::cout << "simulate\n";
        statisticInit();
        BaseModel::simulate();
        statisticEnd();
    }

protected:
    void initParam(std::unordered_map<std::string, double>& mp) {
        //std::cout << "init param\n";
        latent_period = mp["latent_period"];
        infectious_period = mp["infectious_period"];
        prob_transmission = mp["prob_transmission"];
        prob_death = mp["prob_death"];
        prob_immute = mp["prob_immute"];
        vaccine_population = mp["vaccine_population"];
        vaccine_efficiency = mp["vaccine_efficiency"];

        tm.setDay(mp["simulate_day"]);

        I.setAvgPeriod(infectious_period);
        E.setAvgPeriod(latent_period);
    }

    void loadVaccineOrder(std::istream& in_vacc) {
        //std::cout << "load vacc order\n";
        uint n, nid;
        in_vacc >> n;
        for (uint i = 0; i < n; ++i) {
            in_vacc >> nid;
            vacc_order.push_back(Node(&ndp[nid]));
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

    Nodes infection(SuspictibleState& Sus, uint period, double prob_trans) {
        //std::cout << "one infection\n";
        // Nodes sus2i;
        // for (uint i = 0; i < I.size(); ++i) {
        //     Node u = I[i];
        //     for (auto cgp : u.getGroups()[period]) {          
        //         Nodes tmp = Sus.infected(u.getAge(), cgp, prob_transmission);
        //         while (tmp.size()) {
        //             sus2i.push_back(tmp.back());
        //             tmp.pop_back();
        //         }
        //     }
        // }
        // return sus2i;
        //std::cout << "prob trans " << prob_trans << '\n';
        Nodes sus2i;
        for (uint i = 0; i < I.size(); ++i) {
            Nodes tmp = Sus.infected(I[i], period, prob_trans);
            sus2i.insert(sus2i.end(), tmp.begin(), tmp.end());
        }
        return sus2i;
    }

    Nodes vaccination() {
        //std::cout << "vaccination\n";
        static uint vacc_idx = 0;
        Nodes re;
        uint cnt = 0;
        while (cnt < vaccine_population && vacc_idx < vacc_order.size()) {
            Node cur = vacc_order[vacc_idx];
            //std::cout << "try vacc " << cur.getID() << '\n';
            if (cur.getState() == 'S') {
                //std::cout << "vacc " << cur.getID() << '\n';
                S.erase(cur);
                re.push_back(cur);
                ++cnt;
            }
            ++vacc_idx;
        }
        return re;
    }

    void simulate_unit(const Time::TimeStep& ts) {
        std::cout << "----simulate unit " << ts.getDay() << ' ' << ts.getPeriod() << "----\n";
        Nodes s2e = infection(S, ts.getPeriod(), prob_transmission);
        Nodes f2e = infection(F, ts.getPeriod(), prob_transmission);

        Nodes e2i = E.expire();
        Nodes i2frd = I.expire(), i2f, i2r, i2d;
        for (const auto& v : i2frd) {
            if (Random::trail(prob_death)) {
                i2d.push_back(v);
            }
            else if (Random::trail(prob_immute / (1 - prob_death))) {
                i2r.push_back(v);
            }
            else {
                i2f.push_back(v);
            }
        }

        s2e.setState('E');
        for (auto& v : s2e) E.insert(v);

        f2e.setState('E');
        for (auto& v : f2e) E.insert(v);

        e2i.setState('I');
        for (auto& v : e2i) I.insert(v);

        i2f.setState('F');
        for (auto& v : i2f) F.insert(v);

        i2r.setState('R');
        for (auto& v : i2r) R.insert(v);

        i2d.setState('D');
        for (auto& v : i2d) D.insert(v);

        Nodes s2v = vaccination();

        Nodes v2e = infection(V, ts.getPeriod(), prob_transmission * (1 - vaccine_efficiency));

        statisticUnit(ts, v2e, s2v, s2e, f2e, e2i, i2f, i2r, i2d);

        v2e.setState('E');
        for (auto& v : v2e) E.insert(v);

        s2v.setState('V');
        for (auto& v : s2v) V.insert(v);
    }

    // to override
    virtual void statisticInit() = 0;
    virtual void statisticUnit(const Time::TimeStep& ts, const Nodes& v2e, const Nodes& s2v, const Nodes& s2e, const Nodes& f2e, const Nodes& e2i, const Nodes& i2f, const Nodes& i2r, const Nodes& i2d) = 0;
    virtual void statisticEnd() = 0;

    SuspictibleState S, F, V;
    ExpiringState E, I;
    StableState R, D;

    Nodes vacc_order;

    uint vaccine_population;
    double vaccine_efficiency;
    double latent_period;
    double infectious_period;
    double prob_transmission;
    double prob_death;
    double prob_immute;
};

#endif