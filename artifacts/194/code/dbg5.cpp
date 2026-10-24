#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) { int u, v; cin >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    long long ans = 0;
    for (int a = 1; a <= n; a++)
        for (int b = 1; b <= n; b++) {
            if (a == b) continue;
            for (int c = 1; c <= n; c++) {
                if (c == a || c == b) continue;
                vector<char> used(n + 1, 0);
                used[a] = 1;
                function<bool(int)> go = [&](int u) -> bool {
                    if (u == c) return true;
                    if (u == b) {
                        for (auto v : adj[u]) if (!used[v]) { used[v]=1; if (go(v)) return true; used[v]=0; }
                        return false;
                    }
                    for (auto v : adj[u]) {
                        if (v == b) { used[b]=1; if (go(b)) return true; used[b]=0; }
                        else if (!used[v]) { used[v]=1; if (go(v)) return true; used[v]=0; }
                    }
                    return false;
                };
                bool ok = go(a);
                if (ok) { ans++; printf("%d %d %d OK\n", a, b, c); }
            }
        }
    printf("ans=%lld\n", ans);
    return 0;
}
