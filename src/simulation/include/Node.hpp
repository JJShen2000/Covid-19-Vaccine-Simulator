#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

#include "Type.hpp"
#include "Time.hpp"
#include "ContactGroup.hpp"

class ContactGroup;

struct Individual {
    uint id;
    Age age;
    Location loc;
    char stateID;
    std::vector<std::vector<ContactGroup>> gp;
};

class Node {
public:
    Node() = default;
    Node(Individual* v) : inst(v) {}

    inline uint getID() const {
        return inst->id;
    }

    inline Age getAge() const {
        return inst->age;
    }

    inline char getState() const {
        return inst->stateID;
    }

    inline Location getLocation() const {
        return inst->loc;
    }

    inline std::vector<std::vector<ContactGroup>>& getGroups() const {
        return inst->gp;
    }

    inline void setState(char s) {
        inst->stateID = s;
    }

    friend inline bool operator==(const Node& a, const Node& b) {
        return a.getID() == b.getID();
    }

    friend inline bool operator!=(const Node& a, const Node& b) {
        return a.getID() != b.getID();
    }

    friend inline bool operator<(const Node& a, const Node& b) {
        return a.getID() < b.getID();
    }

    friend inline bool operator>(const Node& a, const Node& b) {
        return a.getID() > b.getID();
    }

protected:
    Individual* inst;
};

class Nodes : public std::vector<Node> {
public:
    void setState(char s) {
        for (auto& v : *this) {
            v.setState(s);
        }
    }
};

using NodePool = std::vector<Individual>;

// class NodePool : public std::vector<Individual> {

// };

#endif