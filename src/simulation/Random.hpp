#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <random>
#include <unordered_set>

//include "Debug.hpp"

//#include "Uint.hpp"

class Random {
public:
    inline static bool trail(double d) {
        return std::uniform_real_distribution<double>(0.0, 1.0)(Random::gen) <= d;
    }

    inline static int uniform(int l, int r) {
        return std::uniform_int_distribution<uint>(l, r)(Random::gen);
    }

    inline static std::vector<uint> choose(unsigned int n, unsigned int k) {
        // if (k > n) {
        //     std::cout << "[ERROR] n = " << n << ", k = " << k << '\n'; 
        // }
        std::unordered_set<uint> ch;
        for (unsigned int r = n - k; r < n; ++r) {
            if (!ch.insert(std::uniform_int_distribution<uint>(0, r)(Random::gen)).second) {
                ch.insert(r);
            }
        }
        return std::vector<uint>(ch.begin(), ch.end());
    }

    inline static double exp_dis(double lambda) {
        return std::exponential_distribution<double>(lambda)(Random::gen);
    }

    inline static uint bino_dis(uint n, double p) {
        return std::binomial_distribution<uint>(n, p)(Random::gen);
    }

protected:
    static std::default_random_engine gen;
};

std::default_random_engine Random::gen(time(NULL));

#endif