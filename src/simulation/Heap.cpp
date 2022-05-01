#include "Heap.hpp"
#include <stdio.h>
#include <stdlib.h>

template <typename S, typename I>
void heap<S, I>::siftUp(int victim)
{
    if (victim > size)
    {
        fprintf(stderr, "(siftUp)Index: %d out of bound, heap size: %d", victim, size);
        exit(EXIT_FAILURE);
    }

    int parent;

    while (victim > 1)
    {
        parent = victim >> 1;
        if (nodes[victim].score > nodes[parent].score)
        {
            swap(node_index_map[nodes[victim].index], node_index_map[nodes[parent].index]);
            swap(nodes[victim], nodes[parent]);
        }
        else
            break;

        victim = parent;
    }
}

template <typename S, typename I>
void heap<S, I>::siftDown(int victim)
{
    if (victim > size)
    {
        fprintf(stderr, "(siftDown)Index: %d out of bound, heap size: %d", victim, size);
        exit(EXIT_FAILURE);
    }

    int lchd, rchd;
    int maxchd;

    while ((victim << 1) < size)
    {
        lchd = victim << 1, rchd = lchd | 1;
        maxchd = nodes[rchd].score > nodes[lchd].score ? rchd : lchd;

        if (nodes[maxchd].score > nodes[victim].score)
        {
            swap(node_index_map[nodes[victim].index], node_index_map[nodes[maxchd].index]);
            swap(nodes[victim], nodes[maxchd]);
        }
        else
            break;

        victim = maxchd;
    }
}

template <typename S, typename I>
heap<S, I>::heap(int max_heap_size)
{
    size = 0;
    nodes.push_back(node{0, 0});
    node_index_map.resize(max_heap_size + 1, -1);
}

template <typename S, typename I>
S heap<S, I>::getScoreByIndex(I idx) const
{
    return nodes[node_index_map[idx]].score;
}

template <typename S, typename I>
void heap<S, I>::push(const node &n)
{
    nodes.push_back(n);
    node_index_map[n.index] = ++size;
    siftUp(size);
}

// template <typename S, typename I>
// typename heap<S, I>::node heap<S, I>::top()
// {
//     return nodes[1];
// }

template <typename S, typename I>
S heap<S, I>::top() const
{
    return nodes[1].score;
}

template <typename S, typename I>
typename heap<S, I>::node heap<S, I>::pop()
{
    heap<S, I>::node ret = nodes[1];
    swap(node_index_map[nodes[1].index], node_index_map[nodes[size].index]);
    swap(nodes[1], nodes[size]);
    nodes.pop_back();
    node_index_map[nodes[size].index] = -1;
    --size;
    if (!empty())
        siftDown(1);
    return ret;
}

template <typename S, typename I>
void heap<S, I>::updateScore(const I idx, const S newscore)
{
    if (idx > size)
    {
        fprintf(stderr, "(updateScore)Index: %d out of bound, heap size: %d", idx, size);
        exit(1);
    }

    heap<S, I>::node *temp_node = &nodes[node_index_map[idx]];

    if (newscore == temp_node->score)
        return;
    else if (newscore > temp_node->score)
    {
        temp_node->score = newscore;
        siftUp(node_index_map[idx]);
    }
    else
    {
        temp_node->score = newscore;
        siftDown(node_index_map[idx]);
    }
}

template <typename S, typename I>
void heap<S, I>::increaseScore(const I idx, const S inc)
{
    updateScore(idx, getScoreByIndex(idx) + inc);
}

template <typename S, typename I>
bool heap<S, I>::empty() const
{
    return size == 0;
}