#ifndef SET_HPP
#define SET_HPP

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#include <vector>

#include "Node.hpp"
#include "Random.hpp"

template<typename T>
using index_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;

class NodeContainer : public index_set<uint> {
public:

    inline void insert(uint v) {
        index_set<uint>::insert(v);
        //sz += index_set<Node>::insert(v).second;
    }

    inline void erase(uint v) {
        index_set<uint>::erase(v);
        //sz -= index_set<Node>::erase(v);
        //if (res) --sz;
    }

    inline uint size() const {
        return index_set<uint>::size();
        //return sz;
    }

    // inline bool find(const Node& v) const {
    //     return index_set<Node>::find(v) != index_set<Node>::end();
    // }

    inline std::vector<uint> randomChoose(uint k) {
        //std::cout << "nd ctn choose " << k << '\n';
        //auto ch = Random::choose(size(), k);
        std::vector<uint> re;
        for (auto& v : Random::choose(size(), k)) {
            re.push_back(*index_set<uint>::find_by_order(v));
        }
        return re;
    }

// protected:
//     uint sz = 0;
};

#endif