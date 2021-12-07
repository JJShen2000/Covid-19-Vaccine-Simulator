#include <fstream>
#include <vector>

using namespace std;

const int N_nd = 100;
const int N_pr = 2;
const int N_gp = N_nd - 1;
const int N_ag = 2;
const int N_cm = 1;
const int N_lc = 5;

int main() {
    ofstream fout("../data/graph_path.txt");

    fout << N_nd << ' ' << N_pr << ' ' << N_gp << ' ' << N_ag << ' ' << N_cm << ' ' << N_lc << '\n';

    for (int i = 0; i < N_gp; ++i) {
        fout << 0 << ' ';
    }
    fout << '\n';

    fout << 0.01 << ' ' << 0.05 << '\n' << 0.05 << ' ' << 0.1 << '\n';

    for (int i = 0; i < N_nd; ++i) {
        fout << i % 2 << ' ' << i / (N_nd / N_lc) << '\n';

        vector<int> vec;
        if (i != 0) {
            vec.push_back(i - 1);
        }
        if (i != N_nd - 1) {
            vec.push_back(i);
        }

        for (int j = 0; j < N_pr; ++j) {
            fout << vec.size() << ' ';
            for (auto v : vec) {
                fout << v << ' ';
            }
            fout << '\n';
        }
    }

    fout.close();

    return 0;
}