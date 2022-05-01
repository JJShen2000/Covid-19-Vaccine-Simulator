#ifndef __HEAP_H__
#define __HEAP_H__

#include <limits>
#include <vector>
// #include <unordered_map>
using namespace std;

#define MAX_HEAP_SIZE 25000000

template <typename S, typename I>
class heap {
public:
    struct node {
        S score;
        I index;
    };

private:
    int                         size;
//  unordered_map<int, int>     node_index_map;
    vector<int>                 node_index_map;
    vector<node>                nodes;

    void siftUp(int victim);
    void siftDown(int victim);

public:
    heap(int max_heap_size = MAX_HEAP_SIZE);

    S    getScoreByIndex(I idx) const;

    void push(S score, I index);
    void push(const node &n);
    // node top();
    S top() const;
    node pop();

    void updateScore(const I idx, const S newscore);
    void increaseScore(const I idx, const S inc);
    bool empty() const;
};

#endif