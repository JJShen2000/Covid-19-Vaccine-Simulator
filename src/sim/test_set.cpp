#include <iostream>

#include "Set.hpp"

using namespace std;

void test(const Set<int>& st) {
    static int cnt = 0;
    cout << "test " << ++cnt << '\n';

    cout << st.find(1) << '\n';
    cout << st.find(2) << '\n';
    cout << st.empty() << '\n';
    cout << st.size() << '\n';
}

int main() {
    Set<int> st;

    st.insert(1);
    test(st);

    st.insert(2);
    test(st);

    st.erase(1);
    test(st);

    st.erase(2);
    test(st);

    return 0;
}