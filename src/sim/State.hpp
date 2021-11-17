#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <iostream>

#include "Erasable.h"

template<typename Node>
class BaseState {
public:
    BaseState() = default;

    void insert(const Node&) {
        std::cout << "insert\n";
        _nodes_new_added.push_back(node);
    }

    void flush() {
        std::cout << "flush\n";
        while (!_nodes_new_added.empty()) {
            _add_new_node(_nodes_new_added.back());
            _nodes_new_added.pop_back();
        }
    }
protected:
    virtual void _add_new_node(const Node&) = 0;

private:
    std::vector<Node> _nodes_new_added;
};

template<typename Node>
class StableState : public BaseState<Node> {
public:
    StableState() = default;
    
protected:
    virtual void _add_new_node(const Node&) override {
        std::cout << "OK!\n";
    }

    std::vector<Node> _nodes;
};

#endif