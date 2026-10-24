#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
int n;
int e[20];          // 边 i: (i, e[i])，无向
vector<int> g[20];
int vis[20];
bool hasCycle(int mask) {
    for (int i = 0; i < 20; i++) g[i].clear();
    for (int i = 1; i <= n; i++) {
        int a = i, b = e[i];
        if ((mask >> (i - 1)) & 1) swap(a, b);
        g[a].push_back(b);
    }
    fill(vis, vis + 20, 0);
    // 有向图判环：DFS 灰点
    int color[20] = {0};
    function<bool(int)> dfs = [&](int u) -> bool {
        color[u] = 1;
        for (int v : g[u]) {
            if (color[v] == 1) return true;
            if (color[v] == 0 && dfs(v)) return true;
        }
        color[u] = 2;
        return false;
    };
    for (int i = 1; i <= n; i++) if (color[i] == 0 && dfs(i)) return true;
    return false;
}
int main() {
    int prob; cin >> prob;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> e[i];
    long long cnt = 0;
    for (int mask = 0; mask < (1 << n); mask++) if (!hasCycle(mask)) cnt++;
    cout << cnt % MOD << "\n";
    return 0;
}
