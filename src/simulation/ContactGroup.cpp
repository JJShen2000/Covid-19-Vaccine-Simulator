#include "ContactGroup.hpp"

void Group::setContactMatrix(const ContactMatrix& cmtx) {
    this->cm = cmtx;
}

ContactGroup::ContactGroup() {}

ContactGroup::ContactGroup(Group* gp) {
    this->gp = gp;
}

uint ContactGroup::at(uint i) const {
    return gp->nds[i];
}

uint ContactGroup::getID() const {
    return gp->id;
}

ContactMatrix ContactGroup::getContactMatrix() const {
    return gp->cm;
}

uint ContactGroup::getPeriod() const {
    return gp->period;
}

uint ContactGroup::size() const {
    return gp->nds.size();
}