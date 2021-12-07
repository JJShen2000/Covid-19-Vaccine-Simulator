#include <fstream>
#include <iostream>

using namespace std;

int main() {

    ifstream fin("../data/graph_path.txt");
    if (!fin) {
        cout << "GG\n";
    }

    uint a;
    fin >> a;
    cout << a << '\n';

    return 0;
}