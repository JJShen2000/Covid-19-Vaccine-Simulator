#include "util.hpp"

template<typename T>
T order_statistic(std::vector<T>& vec, uint l, uint r, uint k) {
    if (l == r) return vec[l];
    thread_local std::mt19937_64 gen(std::random_device{}());
    swap(vec[r], vec[std::uniform_int_distribution<int>(l, r)(gen)]);
    T pivot = vec[r];
    int mid = l - 1;
    for (int i = l; i < r; ++i) {
        if (vec[i] <= pivot) {
            swap(vec[++mid], vec[i]);
        }
    }
    swap(vec[++mid], vec[r]);

    // for (int i = 0; i < l; ++i) {
    //     cout << "_ ";
    // }
    // for (int i = l; i < mid; ++i) {
    //     cout << vec[i] << ' ';
    // }
    // cout << vec[mid] << "<";
    // for (int i = mid + 1; i <= r; ++i) {
    //     cout << vec[i] << ' ';
    // }
    // for (int i = r + 1; i < vec.size(); ++i) {
    //     cout << "_ ";
    // }
    // cout << '\n';

    uint llen = mid - l;
    if (k == llen) return vec[mid];
    if (k < llen) return order_statistic(vec, l, mid - 1, k);
    // if (k > llen)
    return order_statistic(vec, mid + 1, r, k - llen - 1);

}

template<typename T>
std::vector<uint> smallest(std::vector<T>& vec, uint k) {
    auto cop = vec;
    T pivot = order_statistic(cop, 0, vec.size() - 1, k);
    // cout << k << "-th is " << pivot << '\n';
    vector<uint> smaller, equal;
    for (uint i = 0; i < vec.size(); ++i) {
        if (vec[i] < pivot) smaller.push_back(i);
        if (vec[i] == pivot && equal.size() + smaller.size() < k) equal.push_back(i);
    }
    while (smaller.size() < k) {
        smaller.push_back(equal.back());
        equal.pop_back();
    }
    return smaller;
}