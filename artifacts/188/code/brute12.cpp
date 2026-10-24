// 题1/2 暴力：DFS 全枚举找字典序最小的欧拉路径
// 用法: brute12 <d|u> < in   (d: 有向输入 n m + 边; u: 无向输入 m + 边)
#include <bits/stdc++.h>
using namespace std;
int n, m, kind;
vector<pair<int,int>> es;
string best;
set<int> nodes;

void dfs(vector<int>& verts, int used, int cur) {
    if (used == (1 << m) - 1) {
        string s;
        for (int v : verts) s += to_string(v) + " ";
        if (best.empty() || s < best) best = s;
        return;
    }
    for (int e = 0; e < m; e++) if (!(used >> e & 1)) {
        int u = es[e].first, v = es[e].second;
        if (kind == 0) {
            if (u == cur) {
                verts.push_back(v);
                dfs(verts, used | (1 << e), v);
                verts.pop_back();
            }
        } else {
            if (u == cur) {
                verts.push_back(v);
                dfs(verts, used | (1 << e), v);
                verts.pop_back();
            }
            if (v == cur) {
                verts.push_back(u);
                dfs(verts, used | (1 << e), u);
                verts.pop_back();
            }
        }
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    kind = (argv[1][0] == 'd' ? 0 : 1);
    if (kind == 0) {
        cin >> n >> m;
    } else {
        n = 500;  // P2731 节点上限
        cin >> m;
    }
    es.resize(m);
    for (int i = 0; i < m; i++) {
        cin >> es[i].first >> es[i].second;
        nodes.insert(es[i].first);
        nodes.insert(es[i].second);
    }
    best.clear();
    if (kind == 0) {
        for (int s = 1; s <= n; s++) {
            vector<int> verts{s};
            dfs(verts, 0, s);
        }
    } else {
        for (int s : nodes) {
            vector<int> verts{s};
            dfs(verts, 0, s);
        }
    }
    if (best.empty()) cout << "No\n";
    else cout << best;
    return 0;
}
