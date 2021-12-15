#ifndef EXPIRINGSTATE_HPP
#define EXPIRINGSTATE_HPP

#include <vector>
#include <utility>

//#include "Uint.hpp"
//#include "BaseState.hpp"
#include "Type.hpp"
#include "Time.hpp"
#include "Random.hpp"

class ExpiringState : public std::vector<std::pair<Node, uint>> {
public:
    void insert(const Node& u) {
#ifdef DEBUG
        std::cout << "--insert state infec " << u.getID() << '\n';
#endif
        uint expt = setExpiringTime(u);
#ifdef DEBUG
        std::cout << "----insert " << u.getID() << " with exp period " << expt << '\n';
#endif
        std::vector<std::pair<Node, uint>>::push_back({u, expt});
    }

    void insert(const Nodes& nds) {
        for (auto& v : nds) {
            insert(v);
        }
    }

    Nodes expire() {
        Nodes exp;
        std::vector<std::pair<Node, uint>> rev;
        for (auto v : *this) {
            if (--v.second) {
                rev.push_back(v);
            }
            else {
                exp.push_back(v.first);
                
            }
        }
        std::vector<std::pair<Node, uint>>::operator=(std::move(rev));
        return exp;
    }

    uint size() const {
        return std::vector<std::pair<Node, uint>>::size();
    }

    Node& operator[](uint i) {
        return std::vector<std::pair<Node, uint>>::at(i).first;
    }

    void setAvgPeriod(double val) {
        avg_rate_m1 = 1 / (val - 1);
    }

protected:
    uint setExpiringTime(const Node&) const {
        return 1 + std::ceil(Random::exp_dis(avg_rate_m1));
    }
    double avg_rate_m1;
};

#endif