#include "ContactGroup.h"
#include "Node.h"

ContactGroup::ContactGroup() {}

ContactGroup::ContactGroup(Group* gp) {
    this->gp = gp;
}

uint ContactGroup::getID() const {
    return gp->id;
}

ContactMatrix ContactGroup::getContactMatrix() const {
    return gp->cm;
}

bool operator==(const ContactGroup& a, const ContactGroup& b) {
    return a.getID() == b.getID();
}

bool operator!=(const ContactGroup& a, const ContactGroup& b) {
    return a.getID() != b.getID();
}

// uint ContactGroup::size() const {
//     return gp->sz;
// }