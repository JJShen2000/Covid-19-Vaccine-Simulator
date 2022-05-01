#include "ContactMatrix.hpp"

#define max(a, b) ((a) > (b))? (a) : (b)

void Matrix::setSize(uint n) {
    cm = std::vector<std::vector<double>>(n, std::vector<double>(n));
}

void Matrix::setRate(Age u, Age v, double p) {
    cm[u][v] = p;
}

void Matrix::computePmax() {
    p_max = cm[0][0];
    for (uint i = 0; i < cm.size(); ++i) {
        for (uint j = 0; j < cm[0].size(); ++j) {
            p_max = max(p_max, cm[i][j]);
        }
    }
}

ContactMatrix::ContactMatrix() {}

ContactMatrix::ContactMatrix(Matrix* v) : inst(v) {}

double ContactMatrix::getRate(Age u, Age v) const {
    return inst->cm[u][v];
}

double ContactMatrix::getPmax() const {
    return inst->p_max;
}