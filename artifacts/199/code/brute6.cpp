#include <bits/stdc++.h>
using namespace std;
int n;
vector<pair<int,int>> g[20];
int vis[20];
long long ans;
void dfsPath(int u, long long cur) {
    ans = max(ans, cur);
    for (auto [v, w] : g[u]) {
        if (!vis[v]) {
            vis[v] = 1;
            dfsPath(v, cur + w);
            vis[v] = 0;
        }
    }
}
int main() {
    int prob; cin >> prob;
    cin >> n;
    for (int u = 1; u <= n; u++) {
        int v, w; cin >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    // 连通块划分
    int block[20]; memset(block, 0, sizeof block);
    long long total = 0;
    for (int st = 1; st <= n; st++) {
        if (block[st]) continue;
        vector<int> comp; queue<int> q;
        q.push(st); block[st] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            comp.push_back(u);
            for (auto [v, w] : g[u]) if (!block[v]) { block[v] = 1; q.push(v); }
        }
        ans = 0;
        for (int s : comp) {
            vis[s] = 1;
            dfsPath(s, 0);
            vis[s] = 0;
        }
        total += ans;
    }
    cout << total << "\n";
    return 0;
}
