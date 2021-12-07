#include <iostream>
#include <cstdlib>

#include <set>
#include "../Set.hpp"

using namespace std;

bool test() {
    cout << "start\n";
    set<int> st1;
    Set<int> st2;

    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int cur = rand() % 100;
        cout << cur << endl;
        if (!st1.count(cur)) {
            if (st2.find(cur)) {
                cout << "--1\n";
                return 0;
            }
            st1.insert(cur);
            st2.insert(cur);
            if (!st2.find(cur)) {
                cout << "--2\n";
                return 0;
            }
        }
        else {
            if (!st2.find(cur)) {
                cout << "--3\n";
                return 0;
            }
            st1.erase(cur);
            st2.erase(cur);
            if (st2.find(cur)) {
                cout << "--4\n";
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    srand(time(0));

    if (test()) {
        cout << "good\n";
    }
    else {
        cout << "fail\n";
    }

    return 0;
}