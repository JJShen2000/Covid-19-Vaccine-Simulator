#ifndef BASEMODEL_HPP
#define BASEMODEL_HPP

//#include <iostream>
#include <unordered_map>
#include <string>

#include "Debug.hpp"

#include "Time.hpp"
#include "ContactMatrix.hpp"

#include "Node.h"
#include "ContactGroup.h"

class BaseModel {
public:

    void simulate() {
#ifdef DEBUG
        LOG("--simulate")
#endif
        for (auto& ts : tm) {
            simulate_unit(ts);
        }
#ifdef DEBUG
        LOG("--end simulate")
#endif
    }

protected:
    virtual void simulate_unit(const Time::TimeStep& ts) = 0;

    void loadGraph(std::istream& in) {
        //std::cout << "load graph\n";
#ifdef DEBUG
        LOG("--load graph")
#endif
        in >> N_nd >> N_pr >> N_gp >> N_ag >> N_cm >> N_lc;

#ifdef DEBUG
        LOG("----assign pool size")
#endif       
#ifdef DEBUG
        LOG("------assign period")
#endif  
        tm.setPeriod(N_pr);
#ifdef DEBUG
        LOG("------assign contact matrix poll")
#endif  
        cmp.resize(N_cm);
#ifdef DEBUG
        LOG("------assign contact group poll")
#endif  
        cgpp.resize(N_gp);
#ifdef DEBUG
        LOG("------assign node poll")
#endif  
        ndp.resize(N_nd);
        //std::cout << "ndp resize " << N_nd << '\n';

#ifdef DEBUG
        LOG("----assign contact matrix")
#endif
        for (uint i = 0; i < N_gp; ++i) {
            int j;
            in >> j;
            cgpp[i].setContactMatrix(ContactMatrix(&cmp[j]));
            cgpp[i].id = i;
        }

#ifdef DEBUG
        LOG("----assign matrix entry")
#endif
        for (uint i = 0; i < N_cm; ++i) {
            cmp[i].setSize(N_ag);
            for (uint j = 0; j < N_ag; ++j) {
                for (uint k = 0; k < N_ag; ++k) {
                    double p;
                    in >> p;
                    cmp[i].setRate(j, k, p);
                }
            }
        }

        //std::cout << "load ndp, cgpp node\n";

#ifdef DEBUG
        LOG("----push state node")
#endif
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
                    cgpp[gid].nodes.push_back(Node(&cur));
                }
            }
        }
    }

    std::unordered_map<std::string, double> loadParam(std::istream& in) const {
        //std::cout << "load param\n";
#ifdef DEBUG
        LOG("--load param")
#endif
        std::string str;
        double val;
        std::unordered_map<std::string, double> mp;
        while (in >> str >> val) {
#ifdef DEBUG
            std::cout << "----load param " << str << " = " << val << '\n';
#endif
            mp[str] = val;
        }
        return mp;
    }

    std::vector<std::pair<Location, uint>> loadInit(std::istream& in) const {
        //std::cout << "load init\n";
#ifdef DEBUG
        LOG("--load init")
#endif
        std::vector<std::pair<Location, uint>> vec;
        Location loc;
        uint num;
        while (in >> loc >> num) {
            vec.push_back({loc, num});
        }
        return vec;
    }

    uint N_nd; // # of nodes
    uint N_pr; // # of periods per day
    uint N_gp; // # of contact groups
    uint N_ag; // # of age groups
    uint N_cm; // # of contact matrices
    uint N_lc; // # of locations

    ContactMatrixPool cmp;
    ContactGroupPool cgpp;
    NodePool ndp;
    Time tm;
};

#endif