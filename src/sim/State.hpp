#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <utility>
#include <iostream>

#include "Erasable.hpp"

template<typename Node>
class BaseState {
public:
    BaseState() = default;

    void insert_async(const Node& node) {
        //std::cout << "insert " << node << '\n';
        _nodes_new_added.push_back(node);
    }

    void flush() {
        //std::cout << "flush\n";
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
    virtual void _add_new_node(const Node& node) override {
        _nodes.push_back(node);
    }

    std::vector<Node> _nodes;
};

template<typename Node>
class ExpiringState : public BaseState<Node> {
public:
    ExpiringState() = default;

    void expire() {
        //std::cout << "expire\n";
        std::vector<Node> reserve;
        for (auto node : _nodes) {
            //std::cout << "is expire " << node << '\n';
            if (!_is_expire(node)) {
                reserve.push_back(node);
                //std::cout << "reserve " << node << '\n';
            }
        }
        this->_nodes = std::move(reserve);
    }
protected:
    virtual bool _is_expire(const Node&) const = 0;

    virtual void _add_new_node(const Node& node) override {
        _nodes.push_back(node);
    }

    std::vector<Node> _nodes;
};

#endif