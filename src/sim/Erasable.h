#pragma once

template<typename Node>
class Erasable {
public:
    virtual erase(const Node&) = 0;
};