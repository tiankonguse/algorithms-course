#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, root;
    cin >> n >> m >> root;
    vector<vector<int>> adj(n + 1);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> dep(n + 1), par(n + 1);
    function<void(int, int)> dfs = [&](int u, int fa) {
        par[u] = fa;
        for (int v : adj[u]) if (v != fa) { dep[v] = dep[u] + 1; dfs(v, u); }
    };
    dfs(root, 0);
    auto lca = [&](int u, int v) -> int {
        if (u == v) return u;
        if (dep[u] < dep[v]) swap(u, v);
        while (dep[u] > dep[v]) u = par[u];
        while (u != v) { u = par[u]; v = par[v]; }
        return u;
    };
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        cout << lca(u, v) << '\n';
    }
    return 0;
}
