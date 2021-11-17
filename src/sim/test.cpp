#include "State.hpp"
#include <iostream>

using namespace std;

int main() {
    StableState<int> R;
    R.insert(1);
    R.flush();

    return 0;
}