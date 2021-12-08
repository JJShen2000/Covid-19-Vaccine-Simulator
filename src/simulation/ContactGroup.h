#ifndef CONTACTGROUP_H
#define CONTACTGROUP_H

#include <vector>

#include "ContactMatrix.hpp"

class Node;

struct Group {
    void setContactMatrix(const ContactMatrix& cmtx) {
        cm = cmtx;
    }

    std::vector<Node> nodes;
    ContactMatrix cm;
    //uint sz;
    uint id;
};

class ContactGroup {
public:
    ContactGroup();

    ContactGroup(Group* gp);

    uint getID() const;

    ContactMatrix getContactMatrix() const;

    //uint size() const;
    friend bool operator==(const ContactGroup&, const ContactGroup&);
    friend bool operator!=(const ContactGroup&, const ContactGroup&);

protected:
    Group* gp;
};

class ContactGroupPool : public std::vector<Group> {


};

#endif