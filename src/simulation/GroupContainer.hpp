#ifndef GROUPCONTAINER_HPP
#define GROUPCONTAINER_HPP

//#include <algorithm>
#include <unordered_set>
#include <cmath>

//#include "Debug.hpp"

//#include "Uint.hpp"
#include "ContactGroup.hpp"
#include "Node.hpp"
#include "Random.hpp"
#include "NodeContainer.hpp"

class GroupContainer : public std::vector<NodeContainer> {
public:
    inline void insert(const Node& u) {
        std::vector<NodeContainer>::at(u.getAge()).insert(u);
        ++sz;
    }

    inline void erase(const Node& u) {
        at(u.getAge()).erase(u);
        --sz;
    }

    inline uint size() const {
        return sz;
    }

    std::vector<uint> choose_hypergeometric(uint k) const {
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
        return amt;
    }

    void infected(const vector<uint>& src_cnt, const std::vector<double>& ptrans, std::unordered_set<uint>& infected_node) {
        for (uint v = 0; v < ageSize(); ++v) {
            auto& nctn = std::vector<NodeContainer>::at(v);
            double p = 1;
            for (uint u = 0; u < ageSize(); ++u) {
                p *= std::pow(1 - cgp.getContactMatrix().getRate(u, v) * ptrans[v], src_cnt[u]);
            }
            for (auto& nd : nctn.randomChoose(Random::bino_dis(nctn.size(), 1 - p))) {
                infected_node.insert(nd.getID());
            }
        }
    }

    // Nodes choose(uint k) {
    //     //std::cout << "gp ctn choose " << k << '\n';
    //     std::vector<uint> amt(ageSize());
    //     for (uint i = 0; i < k; ++i) {
    //         uint all = sz - i;
    //         for (uint j = 0; j < ageSize(); ++j) {
    //             if (Random::trail((at(j).size() - amt[j]) / (double) all)) {
    //                 ++amt[j];
    //                 break;
    //             }
    //             all -= at(j).size() - amt[j];
    //         }
    //     }

    //     Nodes re;
    //     for (uint i = 0; i < ageSize(); ++i) {
    //         if (amt[i]) {
    //             Nodes tmp = at(i).randomChoose(amt[i]);
    //             re.insert(re.end(), tmp.begin(), tmp.end());    
    //         }
    //     }

    //     return re;
    // }

    inline double getPmax(Age src) const {
        double p = 0;
        for (uint i = 0; i < ageSize(); ++i) {
            if (at(i).size()) {
                p = std::max(p, cgp.getContactMatrix().getRate(src, i));
            }
        }
        return p;
    }

    inline void setContactGroup(const ContactGroup& cgp) {
        this->cgp = cgp;
    }

    inline ContactGroup getContactGroup() const {
        return cgp;
    }

    inline uint ageSize() const {
        return std::vector<NodeContainer>::size();
    }
protected:

    // inline bool find(const Node& u) const {
    //     return at(u.getAge()).find(u);
    // }

    ContactGroup cgp;
    uint sz;
};

#endif