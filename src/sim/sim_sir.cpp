#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <utility>
#include <time.h>

#define sim_algo naive_algo

using namespace std;
using node_t = int;
using group_t = int;
using gc_t = int;

struct Group : vector<node_t> {
    size_t sus_cnt = 0;
};

struct GroupClass : vector<Group> {
};

enum State {
    S, I, R
};

struct Node {
    int age;
    State st;
    int length;
    vector<pair<gc_t, group_t>> g[2];
};

int N_nd, N_gc, N_ag;
vector<GroupClass> g_arr;
vector<Node> n_arr;
double prob;

deque<node_t> i_que;
int sim_day = 20, s_length = 3, cur_period;

void naive_algo(int u, int s, int t) {
    for (auto p : n_arr[u].g[s]) {
        int gc = p.first, gid = p.second;
        for (auto v : g_arr[gc][gid]) {
            if (n_arr[v].st == State::S) {
                double trail = rand() / (double)RAND_MAX;
                if (trail < prob) {
                    cout << "node " << u << " infect " << v << '\n';
                    n_arr[v].st = State::I;
                    n_arr[v].length = cur_period + s_length;
                    i_que.push_back(v);
                }
            }
        }
    }
}

void simulate_period(int s, int t) {
    int i_cnt = i_que.size();
    cout << i_que.size() << " infectors to infect\n";
    for (int i = 0; i < i_cnt; ++i) {
        int u = i_que[i];
        sim_algo(u, s, t);
        if (n_arr[u].length == cur_period) {
            cout << "node " << u << " recovered\n";
            n_arr[u].st = State::R;
        }
    }
    while (i_que.size() && n_arr[i_que.front()].length == cur_period) {
        int u = i_que.front();
        i_que.pop_front();
    }
}

void simulate() {
    cout << "START: " << "simulate\n";
    cur_period = 0;
    for (int i = 0; i < sim_day; ++i) {
        cout << "SIM: " << "sim period " << cur_period << '\n';
        simulate_period(0, 1);
        ++cur_period;
        cout << "SIM: " << "sim period " << cur_period << '\n';
        simulate_period(1, 0);
        ++cur_period;
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));
    ifstream fin(argv[1]);
    if (!fin) {
        cout << "Fail to open " << argv[1] << '\n';
        return 1;
    }

    fin >> N_nd >> N_gc >> N_ag;

    n_arr.resize(N_nd);
    g_arr.resize(N_gc);
    for (int i = 0; i < N_gc; ++i) {
        int d;
        fin >> d;
        g_arr[i].resize(d);
    }

    fin >> prob;

    for (int i = 0; i < N_nd; ++i) {
        int a, d_cnt, n_cnt;
        fin >> a;
        fin >> d_cnt;
        int gc, gid;
        n_arr[i].age = a;
        for (int j = 0; j < d_cnt; j++) {
            fin >> gc >> gid;
            n_arr[i].g[0].push_back({gc, gid});
            g_arr[gc][gid].push_back(i);
        }
        fin >> n_cnt;
        for (int j = 0; j < d_cnt; j++) {
            fin >> gc >> gid;
            n_arr[i].g[1].push_back({gc, gid});
            g_arr[gc][gid].push_back(i);
        }
    }

    int s_cnt = 5;
    for (int i = 0; i < s_cnt; ++i) {
        n_arr[i].st = State::I;
        n_arr[i].length = s_length;
        i_que.push_back(i);
    }
    for (int i = s_cnt; i < N_nd; ++i) {
        n_arr[i].st = State::S;
        for (auto p : n_arr[i].g[0]) {
            int gc = p.first, gid = p.second;
            ++g_arr[gc][gid].sus_cnt;
        }
    }

    simulate();

    return 0;
}