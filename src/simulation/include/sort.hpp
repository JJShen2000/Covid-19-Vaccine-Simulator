#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <vector>
#include <thread>

using namespace std;

#define THREAD_NUM 8

template <class T>
void quick_sort(vector<T> &v, int start, int end)
{
    sort(v.begin() + start, v.begin() + end);
}

template <class T>
void merge(vector<T> &v, int l, int m, int r)
{
    vector<T> t1(v.begin() + l, v.begin() + m), t2(v.begin() + m, v.begin() + r);
    int i = 0, j = 0, k = l;
    while (i < t1.size() && j < t2.size())
        if (t1[i] <= t2[j])
            v[k++] = t1[i++];
        else
            v[k++] = t2[j++];
    while (i < t1.size())
        v[k++] = t1[i++];
    while (j < t2.size())
        v[k++] = t2[j++];
}

template <class T>
void parallel_merge_sort(vector<T> &v)
{
    int chunk_size = v.size() / THREAD_NUM;
    vector<thread> threads;

    for (int i = 0; i < THREAD_NUM; ++i)
        threads.push_back(thread(quick_sort<T>, ref(v), i * chunk_size, i == THREAD_NUM - 1 ? v.size() : (i + 1) * chunk_size));
    for (int i = 0; i < THREAD_NUM; ++i)
        threads[i].join();

    threads.clear();

    int cnt = THREAD_NUM;
    while (cnt > 1)
    {
        for (int i = 0; i < cnt; i += 2)
            threads.push_back(thread(merge<T>, ref(v), i * chunk_size, (i + 1) * chunk_size, i == cnt - 2 ? v.size() : (i + 2) * chunk_size));
        
        for (int i = 0; i < (cnt >> 1); ++i)
            threads[i].join();
        threads.clear();

        cnt >>= 1, chunk_size <<= 1;
    }
}
#endif