#ifndef GROUPCONTAINER_HPP
#define GROUPCONTAINER_HPP

//#include <algorithm>
//#include <unordered_set>

#include "Debug.hpp"

//#include "Uint.hpp"
#include "ContactGroup.h"
#include "Node.h"
#include "Random.hpp"
#include "NodeContainer.hpp"


// class GroupContainer {
// public:
//     virtual void insert(const Node&) = 0;
//     virtual void erase(const Node&) = 0;
//     virtual uint size() const = 0;
// protected:
    
// };

class SetAgedGroup : public std::vector<NodeContainer> {
public:
    void insert(const Node& u) {
        at(u.getAge()).insert(u);
        ++sz;
    }

    void erase(const Node& u) {
        at(u.getAge()).erase(u);
        --sz;
    }

    uint size() const {
        return sz;
    }

    Nodes choose(uint k) {
        //std::cout << "gp ctn choose " << k << '\n';
        std::vector<uint> amt(ageSize());
        for (uint i = 0; i < k; ++i) {
            uint all = sz - i;
            for (uint j = 0; j < ageSize(); ++j) {
                if (Random::trail((at(j).size() - amt[j]) / (double) all)) {
                    ++amt[j];
                    break;
                }
                all -= at(j).size() - amt[j];
            }
        }

        Nodes re;
        for (uint i = 0; i < ageSize(); ++i) {
            if (amt[i]) {
                Nodes tmp = at(i).randomChoose(amt[i]);
                re.insert(re.end(), tmp.begin(), tmp.end());    
            }
        }

        return re;
    }

    // Nodes extract(uint k) {
    //     Nodes re;
    //     for (uint i = 0; i < k; ++i) {
    //         uint all = sz;
    //         for (uint j = 0; j < ageSize(); ++j) {
    //             if (at(j).size() && Random::trail(at(j).size() / (double)(all))) {
    //                 re.push_back(at(j).randomExtract());
    //                 break;
    //             }
    //             else {
    //                 all -= at(j).size();
    //             }
    //         }
    //     }
    //     return re;
    // }

    double getPmax(Age u) const {
        double p = 0;
        for (uint i = 0; i < ageSize(); ++i) {
            if (at(i).size()) {
                p = std::max(p, cgp.getContactMatrix().getRate(u, i));
            }
        }
        return p;
    }

    void setContactGroup(const ContactGroup& cgp) {
        this->cgp = cgp;
    }

    ContactGroup getContactGroup() const {
        return cgp;
    }
protected:

    inline uint ageSize() const {
        return std::vector<NodeContainer>::size();
    }

    bool find(const Node& u) const {
        return at(u.getAge()).find(u);
    }

    ContactGroup cgp;
    uint sz;
};

// class VectorAgedGroup : public GroupContainer, public std::vector<Nodes> {
// public:
//     VectorAgedGroup() = default;

//     virtual void insert(const Node& u) override {
//         if (!find(u)) {
//             ++sz;
//             at(u.getAge()).push_back(u);    
//         }
//         //_nodes[v.getAge()].push_back(v);
//     }

//     bool find(const Node& u) const {
//         for (auto& v : at(u.getAge())) {
//             if (u == v) return true;
//         }
//         return false;
//     }

//     virtual void erase(const Node& u) override {
//         --sz;
//         auto& nds = at(u.getAge());
//         for (int i = 0; i < nds.size(); ++i) {
//             if (nds[i] == u) {
//                 nds.erase(nds.begin() + i);
//                 return;
//             }
//         }
//         // --_sz;
//         // for (int i = 0; i < _nodes[v.getAge()].size(); ++i) {
//         //     Node u = _nodes[v.getAge()][i];
//         //     if (u == v) {
//         //         _nodes[v.getAge()].erase(_nodes[v.getAge()].begin() + i);
//         //         return;
//         //     }
//         // }
//     }

//     virtual uint size() const override {
//         return sz;
//     }

//     uint size(Age a) const {
//         return std::vector<Nodes>::at(a).size();
//     }

//     uint ageSize() const {
//         return std::vector<Nodes>::size();
//     }

//     ContactGroup getContactGroup() const {
//         return cgp;
//     }

//     double getPmax(Age u) const {
// #ifdef DEBUG
//         std::cout << "--getting p max: ";
//         for (uint i = 0; i < ageSize(); ++i) {
//             std::cout << cgp.getContactMatrix().getRate(u, i) << ' ';
//         }
//         std::cout << '\n';
// #endif
//         double p = 0;
//         for (uint i = 0; i < ageSize(); ++i) {
//             if (size(i)) {
//                 p = std::max(p, cgp.getContactMatrix().getRate(u, i));
//             }
//         }
//         return p;
//     }

//     Nodes choose(uint k) const {
//         // std::vector<uint> ched(_nodes.size());
//         // uint ageSize = _nodes.size();

//         // for (uint i = 0; i < k; ++i) {
//         //     uint psum = 0, offset = i;
//         //     for (int j = 0; j < ageSize; ++j) {
//         //         if (Random::trail(_nodes[i].size() - ched[i]) / static_cast<double>(_sz - offset - psum)) {
//         //             ++ched[j];
//         //             break;
//         //         }
//         //     }
//         // }

//         // for (int i = 0; i < k; ++i) {
//         //     Node cur = choose_one(ched, i);
//         //     ched[cur.getAge()].insert(cur);
//         // }

//         // Nodes re;
//         // for (int i = 0; i < ched.size(); ++i) {
//         //     std::vector tmp(ched[i].begin(), ched.end());
//         //     while (tmp.size()) {
//         //         re.push_back(tmp.back());
//         //         tmp.pop_back();
//         //     }
//         // }
//         // return re;

//         Nodes re;
//         uint i = 0, j = 0;
//         while (re.size() < k) {
//             if (j == at(i).size()) {
//                 j = 0;
//                 ++i;
//                 continue;
//             }

//             re.push_back(at(i)[j]);
//             ++j;
//         }
//         return re;
//     }

//     void setContactGroup(const ContactGroup& cgp) {
//         this->cgp = cgp;
//     }

// protected:

//     // Node choose_one(const std::vector<std::unordered_set<Node>>& ched, uint offset) const {
//     //     uint psum = 0;
//     //     for (int i = 0; i < _nodes.size(); ++i) {
//     //         if (Random::trail(_nodes[i].size() - ched[i]) / static_cast<double>(_sz - offset - psum)) {
//     //             return _nodes[Random::uniform(0, _nodes[i].size() - 1)];
//     //         }
//     //         psum += _nodes[i].size() - ched[i];
//     //     }
//     // }

//     uint sz = 0;
//     ContactGroup cgp;
// };

#endif