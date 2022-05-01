#ifndef EXPIRINGSTATE_HPP
#define EXPIRINGSTATE_HPP

#include <vector>
#include <utility>
#include <deque>

//#include "Uint.hpp"
//#include "BaseState.hpp"
#include "Type.hpp"
#include "Time.hpp"
#include "Random.hpp"

class ExpiringState : public std::deque<std::vector<uint>> {
public:
    // class Iterator {
    // public:
    //     Iterator();

    //     Iterator(uint i, uint j, const ExpiringState* ext);

    //     uint operator*() const;

    //     Iterator& operator++();

    //     Iterator operator++(int);

    //     bool exist() const;

    //     Iterator next() const;

    //     friend bool operator!=(const Iterator& a, const Iterator& b);

    // protected:
    //     const ExpiringState* ext;
    //     uint i, j;
    // };

    // Iterator begin();

    // Iterator end();
    ExpiringState();

    void insert(uint u);

    std::vector<uint> expire();

    // inline uint size() const {
    //     return std::vector<std::pair<uint, uint>>::size();
    // }

    // inline uint operator[](uint i) {
    //     return std::vector<std::pair<uint, uint>>::at(i).first;
    // }

    void setAvgPeriod(double val);
protected:
    uint setExpiringTime() const;
    double avg_rate_m1;
};

#endif