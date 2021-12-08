#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <random>
#include <unordered_set>

#include "Debug.hpp"

//#include "Uint.hpp"

class Random {
public:
    static bool trail(double d) {
        static std::uniform_real_distribution<double> dis(0.0, 1.0);
        double r = dis(Random::gen);
        return r <= d;
    }

    static int uniform(int l, int r) {
        return std::uniform_int_distribution<uint>(l, r)(Random::gen);
    }

    static std::vector<uint> choose(unsigned int n, unsigned int k) {
        if (k > n) {
            std::cout << "[ERROR] n = " << n << ", k = " << k << '\n'; 
        }
        std::unordered_set<uint> ch;
        for (unsigned int r = n - k; r < n; ++r) {
            uint cur = std::uniform_int_distribution<uint>(0, r)(Random::gen);

            if (!ch.insert(cur).second) {
                ch.insert(r);
            }
        }
        return std::vector<uint>(ch.begin(), ch.end());
    }

    static double exp_dis(double lambda) {
        return std::exponential_distribution<double>(lambda)(Random::gen);
    }

protected:
    static std::default_random_engine gen;
};

std::default_random_engine Random::gen(time(NULL));

#endif