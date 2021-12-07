#include <vector>
#include "../Debug.hpp"

using namespace std;

int main() {
    vector<int> vec = {1, 2, 4, 5, 7, 8};
    int a = 1, b = 2, c = 3;

#ifdef DEBUG
    PRINT(a);
    PRINT(b);
    PRINT(c);

    PRINTA(vec);
#endif
    return 0;
}