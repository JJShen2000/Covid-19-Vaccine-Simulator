#include <fstream>
#include <vector>
#include <iostream>

#include <random>
// #include <filesystem>

using namespace std;

const int N_nd = 100;
const int N_pr = 2;
const int N_gp = N_nd;
const int N_ag = 10;
const int N_cm = 10;
const int N_lc = 1;

int main() {
    // cout << filesystem::current_path() << '\n';
    ofstream fout("./input/graph_10x10.txt");

    fout << N_nd << ' ' << N_pr << ' ' << N_gp << ' ' << N_ag << ' ' << N_cm << ' ' << N_lc << '\n';
    
    for (int i = 0; i < N_gp; ++i) {
        fout << i / 10 << ' ';
    }
    fout << '\n';

    random_device rd;
    default_random_engine gen(rd());
    uniform_real_distribution<double> dis(0, 0.75);

    for (int i = 0; i < N_cm; ++i) {
        for (int j = 0; j < N_ag; ++j) {
            for (int k = 0; k < N_ag; ++k) {
                fout << dis(gen) << ' ';
            }
            fout << '\n';
        }
    }

    for (int i = 0; i < N_nd; ++i) {
        fout << i % 10 << ' ' << 0 << '\n';
        int x = i / 10, y = i % 10;
        for (int j = 0; j < N_pr; ++j) {
            vector<int> vec;
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    int nx = x + k;
                    int ny = y + l;
                    int ni = nx * 10 + ny;
                    if (nx < 10 && ny < 10) {
                        vec.push_back(ni);
                    }
                }
            }
            fout << vec.size() << ' ';
            for (auto v : vec) {
                fout << v << ' ';
            }
            fout << '\n';
        }
    }

    return 0;
}