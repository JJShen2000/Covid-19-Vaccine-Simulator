#include "ExpiringState.hpp"

#include <iostream>
// ExpiringState::Iterator::Iterator() {}

// ExpiringState::Iterator::Iterator(uint i, uint j, const ExpiringState* ext) {
//     this->ext = ext;
//     this->i = i;
//     this->j = j;
// }

// uint ExpiringState::Iterator::operator*() const {
//     return ext->at(i).at(j);
// }

// ExpiringState::Iterator& ExpiringState::Iterator::operator++() {
//     while (i < ext->size() && j + 1 >= ext->at(i).size()) {
//         j = 0;
//         ++i;
//     }
//     return *this;
// }

// ExpiringState::Iterator ExpiringState::Iterator::operator++(int) {
//     Iterator tmp = *this;
//     ++(*this);
//     return tmp;
// }

// bool ExpiringState::Iterator::exist() const {
//     return i < ext->size() && j < ext->at(i).size();
// }

// ExpiringState::Iterator ExpiringState::Iterator::next() const {
//     Iterator tmp = *this;
//     if (!tmp.exist()) ++tmp;
//     return tmp;
// }

// bool operator!=(const ExpiringState::Iterator& a, const ExpiringState::Iterator& b) {
//     return a.i != b.i || a.j != b.j;
// }

// ExpiringState::Iterator ExpiringState::begin() {
//     return Iterator(0, 0, this).next();
// }

// ExpiringState::Iterator ExpiringState::end() {
//     return Iterator(this->size(), 0, this);
// }

ExpiringState::ExpiringState() : std::deque<std::vector<uint>>() {}

void ExpiringState::insert(uint u) {
    uint p = setExpiringTime();
    // std::cout << "period " << p << '\n';
    while (this->size() < p) this->push_back(std::vector<uint>());
    this->at(p - 1).push_back(u);
}

std::vector<uint> ExpiringState::expire() {
    if (this->size() == 0) return std::vector<uint>();
    std::vector<uint> fst = this->front();
    this->pop_front();
    return fst;
}

void ExpiringState::setAvgPeriod(double val) {
    avg_rate_m1 = 1 / (val);
}

uint ExpiringState::setExpiringTime() const {
    return std::ceil(Random::exp_dis(avg_rate_m1));
}