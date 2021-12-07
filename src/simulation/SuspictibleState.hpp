#ifndef SUSPICTIBLESTATE_HPP
#define SUSPICTIBLESTATE_HPP

#include <vector>

#include "Age.hpp"
#include "Random.hpp"

#include "ContactGroup.h"

#include "BaseState.hpp"
#include "GroupContainer.hpp"

class SuspictibleState : public BaseState, public std::vector<SetAgedGroup> {
public:
    void init(uint N_gp, uint N_ag, ContactGroupPool& cgpp) {
        //std::cout << "Sus init\n";
        std::vector<SetAgedGroup>::resize(N_gp);
        for (auto& vcgp : *this) {
            vcgp.resize(N_ag);
        }

        for (uint i = 0; i < std::vector<SetAgedGroup>::size(); ++i) {
            std::vector<SetAgedGroup>::at(i).setContactGroup(ContactGroup(&cgpp[i]));
        }
    }

    virtual void insert(const Node& u) override {
#ifdef DEBUG
        std::cout << "--insert state Sus " << u.getID() << '\n';
#endif
        for (auto& vcgp : u.getGroups()) {
            for (auto& cgp : vcgp) {
                auto& sagp = std::vector<SetAgedGroup>::at(cgp.getID());
                sagp.insert(u);
            }
        }
        //std::vector<VectorAgedGroup>::at(node.getGroup()).insert(node);
    }

    void erase(const Node& u) {
        for (auto& cgps : u.getGroups()) {
            for (auto& cgp : cgps) {
                auto& vagp = std::vector<SetAgedGroup>::at(cgp.getID());
                vagp.erase(u);
            }
        }
    }

    Nodes infected(const Node& src, uint period, double ptrans) {
        //std::cout << "sus infected by " << src.getID() << ", period " << period << '\n';
        Nodes re;
        for (auto cgp : src.getGroups()[period]) {      
            //std::cout << "trace cgp " << cgp.getID() << '\n';
            auto& gctn = at(cgp.getID());
            if (gctn.size() == 0) continue;

            double pmax = gctn.getPmax(src.getAge()) * ptrans;
            int k = std::ceil(pmax * gctn.size());

            if (k == 0) continue;

            Nodes ch = gctn.choose(k);
            for (auto& v : ch) {
                double p = gctn.getContactGroup().getContactMatrix().getRate(src.getAge(), v.getAge()) * ptrans;
                double dom = k / (double)gctn.size();
                //std::cout << "trail prob " << p << ' ' << dom << ' ' << p / dom << '\n';
                if (Random::trail(p / dom)) {
                    //std::cout << "infected " << v.getID() << '\n';
                    erase(v);
                    re.push_back(v);
                }
            }
        }
        return re;
    }

    // Nodes infected(const Node& src, uint period, double ptrans) {
    //     Nodes re;
    //     for (auto cgp : src.getGroups()[period]) {      
    //         auto& gctn = at(cgp.getID());
    //         double pmax = gctn.getPmax(src.getAge()) * ptrans;
    //         double k = std::ceil(pmax * gctn.size());

    //         Nodes ch = gctn.extract(k);
    //         for (auto& v : ch) {
    //             double p = gctn.getContactGroup().getContactMatrix().getRate(src.getAge(), v.getAge()) * ptrans;
    //             if (Random::trail(p / pmax)) {
    //                 erase(v, gctn.getContactGroup());
    //             }
    //             else {
    //                 gctn.insert(v);
    //             }
    //         }
    //     }
    // }

// protected:
    // void erase(const Node& u, const ContactGroup& ex_cgp) {
    //     for (auto& cgps : u.getGroups()) {
    //         for (auto& cgp : cgps) {
    //             if (cgp != ex_cgp) {
    //                 at(cgp.getID()).erase(u);
    //             }
    //         }
    //     }
    // }

//     Nodes infected(Age src, ContactGroup& cgp) {
// #ifdef DEBUG
//         std::cout << "--attempt infected by node of age " << src << " on cgp " << cgp.getID() << '\n';
// #endif   
//         auto& vagp = at(cgp.getID());

//         double pmax = vagp.getPmax(src);
//         double k = std::ceil(pmax * vagp.size());

//         Nodes ch = vagp.choose(k), re;
//         for (auto v : ch) { 
//             double p = vagp.getContactGroup().getContactMatrix().getRate(src, v.getAge());
//             if (Random::trail(p / pmax)) {
// #ifdef DEBUG
//                 LOG("----one infected")
// #endif
//                 re.push_back(v);
//                 vagp.erase(v);
//             }
//         }
//         return re;
//     }

//     Nodes infected(Age src, ContactGroup& cgp, double ptrans) {
//         auto& vagp = std::vector<VectorAgedGroup>::at(cgp.getID());
//         if (vagp.size() == 0) return Nodes();

// #ifdef DEBUG
//         std::cout << "--attempt infected by node of age " << src << " on cgp " << cgp.getID() << " with ptrans " << ptrans << '\n';
// #endif   
//         double pmax = vagp.getPmax(src) * ptrans;
//         double k = std::ceil(pmax * vagp.size());

//         //std::cout << "get pmax " << pmax << '\n';

// #ifdef DEBUG
//         std::cout << "----upper bound k = " << k << ", pmax = " << pmax << '\n';
// #endif
//         Nodes ch = vagp.choose(k), re;
// #ifdef DEBUG
//         std::cout << "---- chosen nodes ";
//         for (auto& v : ch) {
//             std::cout << v.getID() << ' ';
//         }
//         std::cout << '\n';
// #endif        
//         for (auto v : ch) { 
//             double p = vagp.getContactGroup().getContactMatrix().getRate(src, v.getAge()) * ptrans;
//             //std::cout << "p " << p << ' ' << pmax << ' ' << p / pmax << '\n';
// #ifdef DEBUG
//             std::cout << "------attempt infec node " << v.getID() << " with p/pmax " << p << " " << pmax << '\n';
// #endif
//             if (Random::trail(p / pmax)) {
// #ifdef DEBUG
//                 std::cout << "------infected node " << v.getID() << '\n';
// #endif
                
//                 re.push_back(v);
//             }
//         }
//         return re;
//     }
};

#endif