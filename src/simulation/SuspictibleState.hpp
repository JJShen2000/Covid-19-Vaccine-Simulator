#ifndef SUSPICTIBLESTATE_HPP
#define SUSPICTIBLESTATE_HPP

#include <vector>
#include <iostream>

//#include "Age.hpp"
#include "Type.hpp"
#include "Random.hpp"

#include "ContactGroup.hpp"

//#include "BaseState.hpp"
#include "GroupContainer.hpp"

class SuspictibleState : public std::vector<SetAgedGroup> {
public:
    void init(uint N_gp, uint N_ag, ContactGroupPool& cgpp) {
        //std::cout << "Sus init\n";
        std::vector<SetAgedGroup>::resize(N_gp);
        for (auto& vcgp : *this) {
            vcgp.resize(N_ag);
        }

        for (uint i = 0; i < std::vector<SetAgedGroup>::size(); ++i) {
            std::vector<SetAgedGroup>::at(i).setContactGroup(ContactGroup(&cgpp[i]));
        }
    }

    void insert(const Node& u) {
        for (auto& vcgp : u.getGroups()) {
            for (auto& cgp : vcgp) {
                std::vector<SetAgedGroup>::at(cgp.getID()).insert(u);
            }
        }
        //std::vector<VectorAgedGroup>::at(node.getGroup()).insert(node);
    }

    void insert(const Nodes& nds) {
        for (auto& v : nds) {
            insert(v);
        }
    }

    void erase(const Node& u) {
        for (auto& cgps : u.getGroups()) {
            for (auto& cgp : cgps) {
                std::vector<SetAgedGroup>::at(cgp.getID()).erase(u);
            }
        }
    }

    Nodes infected(const Node& src, uint period, double ptrans) {
        //std::cout << "sus infected by " << src.getID() << ", period " << period << '\n';
        Nodes re;
        for (auto& cgp : src.getGroups()[period]) {      
            //std::cout << "trace cgp " << cgp.getID() << '\n';
            auto& gctn = at(cgp.getID());
            if (gctn.size() == 0) continue;

            double pmax = gctn.getPmax(src.getAge()) * ptrans;
            int k = std::ceil(pmax * gctn.size());
            double dom = k / (double)gctn.size();

            Nodes ch = gctn.choose(k);
            for (auto& v : ch) {
                double p = gctn.getContactGroup().getContactMatrix().getRate(src.getAge(), v.getAge()) * ptrans;
                //std::cout << "trail prob " << p << ' ' << dom << ' ' << p / dom << '\n';
                if (Random::trail(p / dom)) {
                    //std::cout << "infected " << v.getID() << '\n';
                    erase(v);
                    re.push_back(v);
                }
            }
        }
        return re;
    }
};

#endif