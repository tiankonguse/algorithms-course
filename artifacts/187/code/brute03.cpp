// 题3暴力：与课程代码相同的解码逻辑，朴素计算距离和
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<int> arr(n + 1);
    for (int i = 1; i <= n; i++) cin >> arr[i];
    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 1, u, v, w; i < n; i++) { cin >> u >> v >> w; adj[u].push_back({v, w}); adj[v].push_back({u, w}); }
    // 全源距离：以每个点为根 DFS
    auto dist = [&](int s, int t) {
        function<ll(int,int)> dfs = [&](int u, int fa) -> ll {
            if (u == t) return 0;
            for (auto [v, w] : adj[u]) if (v != fa) {
                ll d = dfs(v, u);
                if (d >= 0) return d + w;
            }
            return -1;
        };
        return dfs(s, 0);
    };
    ll mask = (1LL << 30) - 1, lastAns = 0;
    for (int i = 1; i <= q; i++) {
        int op; cin >> op;
        if (op == 1) {
            ll a, b, c; cin >> a >> b >> c;
            a ^= lastAns; b ^= lastAns; c ^= lastAns;
            int x = (int)a, y = (int)b, z = (int)c;
            ll sum = 0;
            for (int k = x; k <= y; k++) sum += dist(arr[k], z);
            lastAns = sum;
            cout << sum << '\n';
            lastAns &= mask;
        } else {
            ll a; cin >> a;
            a ^= lastAns;
            int x = (int)a;
            swap(arr[x], arr[x + 1]);
        }
    }
    return 0;
}
