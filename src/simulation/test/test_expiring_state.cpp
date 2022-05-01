#include "State.hpp"
#include <iostream>

using namespace std;

class E : public ExpiringState<int> {
public:
    void show() {
        for (auto node : _nodes) {
            cout << node << ' ';
        }
        cout << '\n';
    }
protected:
    virtual bool _is_expire(const int& node) const override {
        return node % 2;
    }
};

int main() {
    cout << "???\n";
    E e;
    e.insert_async(1);
    e.insert_async(2);
    e.insert_async(3);
    e.insert_async(4);
    e.flush();
    e.expire();
    e.show();

    return 0;
}