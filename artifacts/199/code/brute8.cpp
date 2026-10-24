#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<int> g[20];
int vis[20];
vector<int> seq;
void greedyDfs(int u) {
    seq.push_back(u);
    vis[u] = 1;
    for (int v : g[u]) if (!vis[v]) greedyDfs(v);
}
int main() {
    int prob; cin >> prob;
    cin >> n >> m;
    vector<pair<int,int>> es;
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        es.push_back({u, v});
    }
    vector<int> best;
    if (m == n - 1) {
        for (auto [u, v] : es) { g[u].push_back(v); g[v].push_back(u); }
        for (int i = 1; i <= n; i++) sort(g[i].begin(), g[i].end());
        greedyDfs(1);
        best = seq;
    } else {
        for (int skip = 0; skip < m; skip++) {
            for (int i = 0; i < 20; i++) g[i].clear();
            for (int j = 0; j < m; j++) if (j != skip) {
                g[es[j].first].push_back(es[j].second);
                g[es[j].second].push_back(es[j].first);
            }
            memset(vis, 0, sizeof vis);
            // 连通检查
            vector<int> q{1}; vis[1] = 1;
            for (size_t i = 0; i < q.size(); i++)
                for (int v : g[q[i]]) if (!vis[v]) { vis[v] = 1; q.push_back(v); }
            if ((int)q.size() != n) continue;
            for (int i = 1; i <= n; i++) sort(g[i].begin(), g[i].end());
            seq.clear();
            memset(vis, 0, sizeof vis);
            greedyDfs(1);
            if (best.empty() || seq < best) best = seq;
        }
    }
    for (size_t i = 0; i < best.size(); i++) cout << best[i] << (i + 1 == best.size() ? "\n" : " ");
    return 0;
}
