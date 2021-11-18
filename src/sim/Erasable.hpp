#ifndef ERASABLE_HPP
#define ERASABLE_HPP

template<typename Node>
class Erasable {
public:
    virtual void erase(const Node&) = 0;
};

#endif