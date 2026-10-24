// 题5暴力：枚举 x<y，dis1+dis2+dis3 最大
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<pair<int,ll>>> a1(n + 1), a2(n + 1), a3(n + 1);
    for (int i = 1, u, v; i < n; i++) { ll w; cin >> u >> v >> w; a1[u].push_back({v,w}); a1[v].push_back({u,w}); }
    for (int i = 1, u, v; i < n; i++) { ll w; cin >> u >> v >> w; a2[u].push_back({v,w}); a2[v].push_back({u,w}); }
    for (int i = 1, u, v; i < n; i++) { ll w; cin >> u >> v >> w; a3[u].push_back({v,w}); a3[v].push_back({u,w}); }
    auto pre = [&](const vector<vector<pair<int,ll>>>& adj, vector<ll>& dep, vector<int>& par, vector<int>& depth) {
        function<void(int,int)> dfs = [&](int u, int fa) {
            par[u] = fa;
            for (auto [v,w] : adj[u]) if (v != fa) { dep[v] = dep[u] + w; depth[v] = depth[u] + 1; dfs(v, u); }
        };
        dfs(1, 0);
    };
    vector<ll> d1(n+1), d2(n+1), d3(n+1);
    vector<int> p1(n+1), p2(n+1), p3(n+1), dp1(n+1), dp2(n+1), dp3(n+1);
    pre(a1, d1, p1, dp1); pre(a2, d2, p2, dp2); pre(a3, d3, p3, dp3);
    auto lca = [&](const vector<int>& par, const vector<int>& depth, int u, int v) {
        if (u == v) return u;
        while (depth[u] > depth[v]) u = par[u];
        while (depth[v] > depth[u]) v = par[v];
        while (u != v) { u = par[u]; v = par[v]; }
        return u;
    };
    ll best = LLONG_MIN;
    for (int x = 1; x <= n; x++) for (int y = x + 1; y <= n; y++) {
        ll val = 0;
        val += d1[x] + d1[y] - 2 * d1[lca(p1, dp1, x, y)];
        val += d2[x] + d2[y] - 2 * d2[lca(p2, dp2, x, y)];
        val += d3[x] + d3[y] - 2 * d3[lca(p3, dp3, x, y)];
        if (val > best) best = val;
    }
    cout << best << '\n';
    return 0;
}
