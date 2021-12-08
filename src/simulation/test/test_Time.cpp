#include <iostream>

#include "Time.hpp"

using namespace std;

int main() {

    Time tm(10, 5);

    for (auto t : tm) {
        cout << t.getDay() << ' ' << t.getPeriod() << '\n';
    }

    return 0;
}