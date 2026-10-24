#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1000000007;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<pair<int,ll>>> adj(n + 1);
    vector<vector<int>> col(n + 1);
    for (int i = 1, u, v, w, c; i < n; i++) {
        cin >> u >> v >> w >> c;
        adj[u].push_back({v, w}); adj[v].push_back({u, w});
        col[u].push_back(c); col[v].push_back(c);
    }
    ll ans = 1;
    bool any = false;
    // 枚举 x < y，DFS 统计路径红/黑与乘积
    for (int x = 1; x <= n; x++) {
        for (int y = x + 1; y <= n; y++) {
            // BFS/DFS from x to y
            function<void(int,int,int,int,ll)> dfs = [&](int u, int fa, int r, int b, ll prod) {
                if (u == y) {
                    if (2 * min(r, b) >= max(r, b)) { any = true; ans = ans * prod % MOD; }
                    return;
                }
                for (int i = 0; i < (int)adj[u].size(); i++) {
                    int v = adj[u][i].first;
                    if (v == fa) continue;
                    dfs(v, u, r + (col[u][i] == 0), b + (col[u][i] == 1), prod * adj[u][i].second % MOD);
                }
            };
            dfs(x, 0, 0, 0, 1);
        }
    }
    cout << (any ? ans : 1) << '\n';
    return 0;
}
