#ifndef SIRMODEL_HPP
#define SIRNODEL_HPP

#include <cmath>
#include <algorithm>
#include <iostream>

#include "Debug.hpp"

#include "Uint.hpp"
#include "Time.hpp"
#include "Random.hpp"

#include "Node.h"
#include "ContactGroup.h"

//#include "State.hpp"
#include "StableState.hpp"
#include "ExpiringState.hpp"
#include "SuspictibleState.hpp"
#include "BaseModel.hpp"

class SIRModel final : public BaseModel {
public:
    SIRModel() = default;

    void load(std::istream& in_graph, std::istream& in_param, std::istream& in_init) {
#ifdef DEBUG
        LOG("--load")
#endif
        loadGraph(in_graph);
        auto mp = loadParam(in_param);

        prob_transmission = mp["prob_transmission"];
        infectious_period = mp["infectious_period"];
        tm.setDay(mp["simulate_day"]);

        I.setAvgPeriod(infectious_period);

        S.init(N_gp, N_ag, cgpp);
        setInitInfectors(loadInit(in_init));
    }   

    void statistic(const Nodes& s2i, const Nodes& i2r) {
        std::cout << "statistic " << s2i.size() << ' ' << i2r.size() << '\n';
    }

protected:
    void setInitInfectors(const std::vector<std::pair<Location, uint>>& loc2num) {
#ifdef DEBUG
        LOG("--set init")
#endif
        std::vector<Nodes> vec(N_lc);
        std::vector<uint> num(N_lc);
        for (auto& u : ndp) {
            vec[u.loc].push_back(Node(&u));
        }
        for (auto& p : loc2num) {
            num[p.first] = p.second;
        }

#ifdef DEBUG
        LOG("----start set init")
#endif
        for (uint i = 0; i < N_lc; ++i) {
            auto& nds = vec[i];
#ifdef DEBUG
            if (vec[i].size() < num[i]) {
                LOG("------[ERROR] too many init infec for loc")
            }
            if (num[i] != 0) {
                std::cout << "------ need " << num[i] << " nodes in loc " << i << '\n';
            }
#endif
            std::vector<uint> ch = Random::choose(vec[i].size(), num[i]);
            std::sort(ch.begin(), ch.end());
            int j = 0;
            for (auto k : ch) {
                while (j < k) {
                    S.insert(nds[j++]);
                }
                ++j;
                I.insert(nds[k]);
            }
            while (j < nds.size()) S.insert(nds[j++]);
        }   
    }

    void simulate_unit(const Time::TimeStep& ts) {
#ifdef DEBUG
        std::cout << "--simulate unit day " << ts.getDay() << ", period " << ts.getPeriod() << '\n';
#endif
        Nodes s2i;
        for (uint i = 0; i < I.size(); ++i) {
            Node u = I[i];
#ifdef DEBUG
            std::cout << "----judge src node " << u.getID() << '\n';
#endif            
            for (auto cgp : u.getGroups()[ts.getPeriod()]) {
// #ifdef DEBUG
//                 std::cout << "------judge with cgp " << cgp.getID() << '\n';
// #endif                 
                Nodes tmp = S.infected(u.getAge(), cgp, prob_transmission);
                while (tmp.size()) {
                    s2i.push_back(tmp.back());
                    S.erase(tmp.back());
                    tmp.pop_back();
                }
            }
        }

        Nodes i2r = I.expire();

#ifdef DEBUG
        if (s2i.size()) {
            std::cout << "----s2i: ";
            for (auto& nd : s2i) {
                std::cout << nd.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (i2r.size()) {
            std::cout << "----i2r: ";
            for (auto& nd : i2r) {
                std::cout << nd.getID() << ' ';
            }
            std::cout << '\n';    
        }
#endif

        statistic(s2i, i2r);

        for (auto& v : s2i) {
            I.insert(v);
        }
        for (auto& v : i2r) {
            R.insert(v);
        }
    }

    SuspictibleState S;
    ExpiringState I;
    StableState R;

    double prob_transmission;
    double infectious_period;
};

#endif