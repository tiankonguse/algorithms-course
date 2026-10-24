#include <bits/stdc++.h>
using namespace std;
int n;
struct E { int u, v, w; };
vector<E> es;
vector<int> g[20];
int vis[20];
void dfsTree(int u, int fa, int &far, int &maxd, int cur) {
    if (cur > maxd) { maxd = cur; far = u; }
    for (auto e : es) {
        int v = -1;
        if (e.u == u && e.v != fa) v = e.v;
        else if (e.v == u && e.u != fa) v = e.u;
        if (v >= 0 && vis[v] == 0) {
            vis[v] = 1;
            dfsTree(v, u, far, maxd, cur + e.w);
        }
    }
}
int diameter() {
    memset(vis, 0, sizeof vis);
    int far = 1, maxd = 0;
    vis[1] = 1; dfsTree(1, 0, far, maxd, 0);
    memset(vis, 0, sizeof vis);
    int far2 = far; maxd = 0;
    vis[far] = 1; dfsTree(far, 0, far2, maxd, 0);
    return maxd;
}
bool connected(int skip) {
    memset(vis, 0, sizeof vis);
    vector<int> q; q.push_back(1); vis[1] = 1;
    for (size_t i = 0; i < q.size(); i++) {
        int u = q[i];
        for (size_t j = 0; j < es.size(); j++) {
            if ((int)j == skip) continue;
            int v = -1;
            if (es[j].u == u) v = es[j].v;
            else if (es[j].v == u) v = es[j].u;
            if (v >= 0 && !vis[v]) { vis[v] = 1; q.push_back(v); }
        }
    }
    for (int i = 1; i <= n; i++) if (!vis[i]) return false;
    return true;
}
int main() {
    int prob; cin >> prob;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int u, v, w; cin >> u >> v >> w;
        es.push_back({u, v, w});
    }
    int best = INT_MAX;
    for (int skip = 0; skip < n; skip++) {
        if (!connected(skip)) continue;
        // 删 skip 边后是树，求直径
        // 直径计算需要排除 skip 边：把 skip 边移走再求
        vector<E> saved = es;
        es.erase(es.begin() + skip);
        best = min(best, diameter());
        es = saved;
    }
    cout << best << "\n";
    return 0;
}
