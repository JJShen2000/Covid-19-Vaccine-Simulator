#ifndef BASESTATE_HPP
#define BASESTATE_HPP

//#include <vector>

#include "Node.h"

class BaseState {
public:

    virtual void insert(const Node& node) = 0;
};

#endif