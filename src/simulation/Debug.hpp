#ifndef DEBUG_HPP
#define DEBUG_HPP

//#define DEBUG

#include <iostream>
#define LOG(x) std::cout << x << '\n';
#define PRINT(x) std::cout << #x << ' ' << x << '\n';
#define PRINTA(x) std::cout << #x << ": "; for (auto& u : x) std::cout << u << ' '; std::cout << '\n';

#endif