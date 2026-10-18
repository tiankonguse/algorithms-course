#include <bits/stdc++.h>
using namespace std;
// 墨墨的等式 P2371 (dijkstra 版)：同余图最短路 + 区间计数
int n;
long long l, r;
int v[13];
long long dist[500001];
bool vis[500001];
long long cnt(long long bound) {
    long long s = 0;
    for (int i = 0; i < v[0]; i++) {
        if (dist[i] <= bound) s += (bound - dist[i]) / v[0] + 1;
    }
    return s;
}
int main() {
    cin >> n >> l >> r;
    for (int i = 0; i < n; i++) cin >> v[i];
    if (n == 0) { cout << 0 << endl; return 0; }
    l -= 1;
    for (int i = 0; i < v[0]; i++) { dist[i] = LLONG_MAX; vis[i] = false; }
    dist[0] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, 0});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int u = top.second;
        if (vis[u]) continue;
        vis[u] = true;
        for (int i = 0; i < n; i++) {
            if (v[i] == 0) continue;
            int nxt = (u + v[i]) % v[0];
            if (dist[nxt] > dist[u] + v[i]) { dist[nxt] = dist[u] + v[i]; pq.push({dist[nxt], nxt}); }
        }
    }
    cout << cnt(r) - cnt(l) << endl;
    return 0;
}
