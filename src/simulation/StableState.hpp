#ifndef STABLESTATE_HPP
#define STABLESTATE_HPP

#include <vector>

#include "BaseState.hpp"

class StableState : public BaseState, Nodes {
public:

    virtual void insert(const Node& node) override {
#ifdef DEBUG
        std::cout << "--insert state Stable " << node.getID() << '\n';
#endif
        push_back(node);
    }

};

#endif