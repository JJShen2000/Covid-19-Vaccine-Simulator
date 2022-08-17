#include "Node.hpp"

Node::Node() {}

Node::Node(Individual* v) :inst(v) {}

inline uint Node::getID() const {
    return inst->id;
}

inline Age Node::getAge() const {
    return inst->age;
}

inline char Node::getState() const {
    return inst->stateID;
}

inline Location Node::getLocation() const {
    return inst->loc;
}

inline std::vector<ContactGroup>& Node::getGroups(uint period) const {
    return inst->gp[period];
}

// inline Time::TimeStep Node::getStartTime() const {
//     return inst->ts;
// }

// inline void Node::setStartTime(const Time::TimeStep& ts) {
//     inst->ts = ts;
// }

inline void Node::setState(char s) {
    inst->stateID = s;
}

inline bool operator==(const Node& a, const Node& b) {
    return a.getID() == b.getID();
}

inline bool operator!=(const Node& a, const Node& b) {
    return a.getID() != b.getID();
}

inline bool operator<(const Node& a, const Node& b) {
    return a.getID() < b.getID();
}

inline bool operator>(const Node& a, const Node& b) {
    return a.getID() > b.getID();
}

// void Nodes::setState(char s) {
//     for (auto& v : *this) {
//         v.setState(s);
//     }
// }