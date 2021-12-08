#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    ofstream fout("./data/vacc_SFEIRVD.txt");
    if (!fout) {
        cout << "GG\n";
        return 0;
    }

    const int N = 100;
    fout << N << '\n';
    vector<int> vec;
    for (int i = 0; i < N; ++i) {
        vec.push_back(i);
    }

    random_shuffle(vec.begin(), vec.end());
    for (auto v : vec) {
        fout << v << ' ';
    }
    fout.close();
    cout << "ok\n";

    return 0;
}