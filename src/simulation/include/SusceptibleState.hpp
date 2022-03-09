#ifndef SUSCEPTIBLESTATE_HPP
#define SUSCEPTIBLESTATE_HPP

#include <vector>
#include <iostream>

//#include "Age.hpp"
#include "Type.hpp"
#include "Random.hpp"

#include "ContactGroup.hpp"

//#include "BaseState.hpp"
#include "GroupContainer.hpp"

class SuspictibleState : public std::vector<GroupContainer> {
public:
    inline void init(uint N_gp, uint N_ag, ContactGroupPool& cgpp) {
        //std::cout << "Sus init\n";
        std::vector<GroupContainer>::resize(N_gp);
        for (auto& vcgp : *this) {
            vcgp.resize(N_ag);
        }

        for (uint i = 0; i < std::vector<GroupContainer>::size(); ++i) {
            std::vector<GroupContainer>::at(i).setContactGroup(ContactGroup(&cgpp[i]));
        }
    }

    inline void insert(const Node& u) {
        for (auto& vcgp : u.getGroups()) {
            for (auto& cgp : vcgp) {
                std::vector<GroupContainer>::at(cgp.getID()).insert(u);
            }
        }
        //std::vector<VectorAgedGroup>::at(node.getGroup()).insert(node);
    }

    inline void insert(const Nodes& nds) {
        for (auto& v : nds) {
            insert(v);
        }
    }

    inline void erase(const Node& u) {
        for (auto& cgps : u.getGroups()) {
            for (auto& cgp : cgps) {
                std::vector<GroupContainer>::at(cgp.getID()).erase(u);
            }
        }
    }

    // Nodes infected(const Node& src, uint period, const std::vector<double>& ptrans) {
    //     //std::cout << "sus infected by " << src.getID() << ", period " << period << '\n';
    //     Nodes re;
    //     //std::cout << "src " << src.getID() << '\n';
    //     uint srcAge = src.getAge();
    //     for (auto& cgp : src.getGroups()[period]) {      
    //         //std::cout << "trace cgp " << cgp.getID() << '\n';
    //         auto& gctn = std::vector<GroupContainer>::at(cgp.getID());
    //         if (gctn.size() == 0) continue;

    //         for (uint i = 0; i < gctn.ageSize(); ++i) {
    //             double p = gctn.getContactGroup().getContactMatrix().getRate(srcAge, i) * ptrans[i];
    //             uint n = Random::bino_dis(gctn[i].size(), p);
    //             if (n == 0) continue;
    //             //std::cout << gctn[i].size() << ' ' << p << ' ' << n << '\n';
                
    //             Nodes ch = gctn[i].randomChoose(n);
    //             for (auto& v : ch) {
    //                 erase(v);
    //             }
    //             re.insert(re.end(), ch.begin(), ch.end());
    //         }

    //         // double pmax = gctn.getPmax(src.getAge()) * ptrans;
    //         // uint k = std::ceil(pmax * gctn.size());
    //         // double dom = k / (double)gctn.size();

    //         // std::vector<uint> amt = gctn.choose_hypergeometric(k);
    //         // for (uint i = 0; i < amt.size(); ++i) {
    //         //     double p = gctn.getContactGroup().getContactMatrix().getRate(src.getAge(), i) * ptrans[i];
    //         //     uint n = Random::bino_dis(amt[i], p / dom);
    //         //     // uint n = amt[i];
    //         //     // for (uint j = 0; j < amt[i]; ++j) {
    //         //     //     double p = gctn.getContactGroup().getContactMatrix().getRate(src.getAge(), i) * ptrans;
    //         //     //     n -= Random::trail(1 - p / dom);
    //         //     //     // if (Random::trail(1 - p / dom)) {
    //         //     //     //     --n;
    //         //     //     // }
    //         //     // }
    //         //     if (n == 0) continue;
    //         //     //std::cout << 'n' << n << '\n';

    //         //     auto ch = gctn[i].randomChoose(n);
    //         //     for (auto& v : ch) {
    //         //         //gctn[i].erase(v);
    //         //         erase(v);
    //         //         //std::cout << "erase " << i << ' ' << v.getID() << '\n';
    //         //     }
    //         //     re.insert(re.end(), ch.begin(), ch.end());
    //         // }

    //     }
    //     return re;
    // }
};

#endif