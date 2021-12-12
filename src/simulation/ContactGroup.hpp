#ifndef CONTACTGROUP_HPP
#define CONTACTGROUP_HPP

#include <vector>

#include "ContactMatrix.hpp"

//class Node;

struct Group {
    void setContactMatrix(const ContactMatrix& cmtx) {
        cm = cmtx;
    }

    ContactMatrix cm;
    uint id;
};

class ContactGroup {
public:
    ContactGroup() {}

    ContactGroup(Group* gp) {
        this->gp = gp;
    }

    inline uint getID() const {
        return gp->id;
    }

    inline ContactMatrix getContactMatrix() const {
        return gp->cm;
    }

    //uint size() const;
    // friend bool operator==(const ContactGroup&, const ContactGroup&);
    // friend bool operator!=(const ContactGroup&, const ContactGroup&);

protected:
    Group* gp;
};

using ContactGroupPool = std::vector<Group>;

// class ContactGroupPool : public std::vector<Group> {


// };

#endif