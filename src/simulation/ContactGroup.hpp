#ifndef CONTACTGROUP_HPP
#define CONTACTGROUP_HPP

#include <vector>

#include "ContactMatrix.hpp"

//class Node;

struct Group {
    void setContactMatrix(const ContactMatrix& cmtx);

    ContactMatrix cm;
    uint id;
    uint period;
    std::vector<uint> nds;
};

class ContactGroup {
public:
    ContactGroup();

    ContactGroup(Group* gp);

    uint at(uint i) const;

    uint getID() const;

    ContactMatrix getContactMatrix() const;

    uint getPeriod() const;

    uint size() const;

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