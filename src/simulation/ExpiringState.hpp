#ifndef EXPIRINGSTATE_HPP
#define EXPIRINGSTATE_HPP

#include <vector>
#include <utility>

//#include "Uint.hpp"
//#include "BaseState.hpp"
#include "Type.hpp"
#include "Time.hpp"
#include "Random.hpp"

class ExpiringState : public std::vector<std::pair<uint, uint>> {
public:
    inline void insert(int u) {
        std::vector<std::pair<uint, uint>>::push_back({u, setExpiringTime()});
    }

    std::vector<uint> expire() {
        std::vector<uint> exp;
        std::vector<std::pair<uint, uint>> rev;
        for (auto v : *this) {
            if (--v.second) {
                rev.push_back(v);
            }
            else {
                exp.push_back(v.first);
                
            }
        }
        std::vector<std::pair<uint, uint>>::operator=(std::move(rev));
        return exp;
    }

    inline uint size() const {
        return std::vector<std::pair<uint, uint>>::size();
    }

    inline uint operator[](uint i) {
        return std::vector<std::pair<uint, uint>>::at(i).first;
    }

    inline void setAvgPeriod(double val) {
        avg_rate_m1 = 1 / (val - 1);
    }

protected:
    uint setExpiringTime() const {
        return 1 + std::ceil(Random::exp_dis(avg_rate_m1));
    }
    double avg_rate_m1;
};

#endif