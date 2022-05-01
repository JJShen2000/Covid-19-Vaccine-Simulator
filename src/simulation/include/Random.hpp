#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <random>
#include <unordered_set>

//include "Debug.hpp"

//#include "Uint.hpp"

class Random {
public:
    static bool trial(double d);

    // inline static int uniform(int l, int r) {
    //     return std::uniform_int_distribution<uint>(l, r)(Random::gen);
    // }

    static std::vector<uint> choose(unsigned int n, unsigned int k);

    static double exp_dis(double lambda);

    static uint bino_dis(uint n, double p);

//protected:
//    static std::default_random_engine gen;
};

//std::default_random_engine Random::gen(time(NULL));

#endif