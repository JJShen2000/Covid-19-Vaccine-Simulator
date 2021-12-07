#include "Node.h"
#include "ContactGroup.h"

Node::Node(Individual* v) : inst(v) {}

uint Node::getID() const {
    return inst->id;
}

Age Node::getAge() const {
    return inst->age;
}

char Node::getState() const {
    return inst->stateID;
}

void Node::setState(char s) {
    inst->stateID = s;
}

Location Node::getLocation() const {
    return inst->loc;
}

std::vector<std::vector<ContactGroup>>& Node::getGroups() const {
    return inst->gp;
}

bool operator==(const Node& a, const Node& b) {
    return a.getID() == b.getID();
}

bool operator!=(const Node& a, const Node& b) {
    return a.getID() != b.getID();
}

bool operator<(const Node& a, const Node& b) {
    return a.getID() < b.getID();
}

bool operator>(const Node& a, const Node& b) {
    return a.getID() > b.getID();
}

void Nodes::setState(char s) {
    for (auto& v : *this) {
        v.setState(s);
    }
}