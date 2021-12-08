#ifndef EXPIRINGSTATE_HPP
#define EXPIRINGSTATE_HPP

#include <vector>
#include <utility>

#include "Uint.hpp"
#include "BaseState.hpp"
#include "Time.hpp"

class ExpiringState : public BaseState, std::vector<std::pair<Node, uint>> {
public:
    virtual void insert(const Node& u) override {
#ifdef DEBUG
        std::cout << "--insert state infec " << u.getID() << '\n';
#endif
        uint expt = setExpiringTime(u);
#ifdef DEBUG
        std::cout << "----insert " << u.getID() << " with exp period " << expt << '\n';
#endif
        std::vector<std::pair<Node, uint>>::push_back({u, expt});
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
        avg_period = val;
    }

protected:
    uint setExpiringTime(const Node&) const {
        return std::ceil(Random::exp_dis(1 / avg_period));
    }
    double avg_period;
};

#endif