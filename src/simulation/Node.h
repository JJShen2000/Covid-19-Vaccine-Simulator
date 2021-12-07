#ifndef NODE_H
#define NODE_H

#include <vector>

#include "Uint.hpp"
#include "Age.hpp"
#include "Location.hpp"
#include "Time.hpp"

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
    Node(Individual* v);

    uint getID() const;

    Age getAge() const;

    char getState() const;

    Location getLocation() const;

    std::vector<std::vector<ContactGroup>>& getGroups() const;

    void setState(char s);

    friend bool operator==(const Node& a, const Node& b);

    friend bool operator!=(const Node& a, const Node& b);

    friend bool operator<(const Node& a, const Node& b);

    friend bool operator>(const Node&, const Node&);

protected:
    Individual* inst;
};

class Nodes : public std::vector<Node> {
public:
    void setState(char s);
};

class NodePool : public std::vector<Individual> {

};

#endif