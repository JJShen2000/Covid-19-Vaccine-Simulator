#ifndef CONTACTMATRIX_HPP
#define CONTACTMATRIX_HPP

#include <vector>

#include "Type.hpp"

struct Matrix {
    inline void setSize(uint n) {
        cm = std::vector<std::vector<double>>(n, std::vector<double>(n));
    }

    inline void setRate(Age u, Age v, double p) {
        cm[u][v] = p;
    }

    std::vector<std::vector<double>> cm;
};

class ContactMatrix {
public:
    ContactMatrix() = default;
    ContactMatrix(Matrix* v) : inst(v) {}

    inline double getRate(Age u, Age v) const {
        return inst->cm[u][v];
    }

protected:
    Matrix* inst;
};

using ContactMatrixPool = std::vector<Matrix>;

// class ContactMatrixPool : public std::vector<Matrix> {
// public:

// };

#endif