#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    vector<vector<int>> adj(n + 1);
    for (int i = 1, u, v; i < n; i++) { cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    ll best = 0;
    // 1) 所有至少 2 个节点的链：指标 = (边数+1) * 最小点权
    for (int s = 1; s <= n; s++) {
        function<void(int,int,int,int,int)> dfs = [&](int u, int fa, int mn, int ecnt, int start) {
            if (u != start) best = max(best, 1LL * (ecnt + 1) * mn);
            for (int v : adj[u]) if (v != fa) dfs(v, u, min(mn, a[v]), ecnt + 1, start);
        };
        dfs(s, 0, a[s], 0, s);
    }
    // 2) 三度化只拆原树度数 >= 3 的点，这些点的单点会经 u-虚点 虚边进入答案
    for (int u = 1; u <= n; u++) {
        if ((int)adj[u].size() >= 3) best = max(best, 1LL * a[u]);
    }
    cout << best << '\n';
    return 0;
}
