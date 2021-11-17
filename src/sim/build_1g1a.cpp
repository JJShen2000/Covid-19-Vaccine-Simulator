#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    ofstream fout(argv[1]);
    int N_node = atoi(argv[2]);
    
    fout << N_node << " 1 1\n";
    fout << "1\n";
    fout << "0.5\n";
    for (int i = 0; i < N_node; ++i) {
        fout << "0 1 0 0 1 0 0\n";
    }

    fout.close();

    return 0;
}