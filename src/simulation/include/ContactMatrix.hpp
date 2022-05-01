#ifndef CONTACTMATRIX_HPP
#define CONTACTMATRIX_HPP

#include <vector>


#include "Type.hpp"

struct Matrix {
    void setSize(uint n);

    void setRate(Age u, Age v, double p);

    void computePmax();

    void prodPtrans(const std::vector<double>& ptrans) {
        for (uint i = 0; i < cm.size(); ++i) {
            for (uint j = 0; j < cm[0].size(); ++j) {
                cm[i][j] *= ptrans[j];
            }
        }
    }

    double p_max;
    std::vector<std::vector<double>> cm;
};

class ContactMatrix {
public:
    ContactMatrix();
    ContactMatrix(Matrix* v);

    double getRate(Age u, Age v) const;

    double getPmax() const;

protected:
    Matrix* inst;
};

using ContactMatrixPool = std::vector<Matrix>;

// class ContactMatrixPool : public std::vector<Matrix> {
// public:

// };

#endif