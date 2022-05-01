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
    // std::vector<ContactGroup> gp;
    Time::TimeStep ts;
    std::vector<std::vector<ContactGroup>> gp;
};

class Node {
public:
    Node();
    Node(Individual* v);

    inline uint getID() const;

    inline Age getAge() const;

    inline char getState() const;

    inline Location getLocation() const;

    inline std::vector<ContactGroup>& getGroups(uint period) const;
    
    inline Time::TimeStep getStartTime() const;

    inline void setStartTime(const Time::TimeStep& ts);

    inline void setState(char s);

    friend inline bool operator==(const Node& a, const Node& b);

    friend inline bool operator!=(const Node& a, const Node& b);

    friend inline bool operator<(const Node& a, const Node& b);

    friend inline bool operator>(const Node& a, const Node& b);

protected:
    Individual* inst;
};

class Nodes : public std::vector<Node> {
public:
    void setState(char s);
};

using NodePool = std::vector<Individual>;

// class NodePool : public std::vector<Individual> {

// };

#endif