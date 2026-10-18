#include <bits/stdc++.h>
using namespace std;
// 牛场围栏 P2662：同余最短路(dijkstra)，模数取最短可用长度
int n, m;
int arr[101];
long long dist[3001];
bool vis[3001];
int main() {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> arr[i];
    bool usable[3001];
    fill(usable, usable + 3001, false);
    int x = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = max(1, arr[i] - m); j <= arr[i]; j++) {
            if (!usable[j]) {
                usable[j] = true;
                if (x == 0) x = j;
            }
        }
    }
    if (x == 1) { cout << -1 << endl; return 0; }
    for (int i = 0; i < x; i++) { dist[i] = LLONG_MAX; vis[i] = false; }
    dist[0] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, 0});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int u = top.second;
        if (vis[u]) continue;
        vis[u] = true;
        for (int j = 1; j <= 3000; j++) {
            if (!usable[j]) continue;
            int nxt = (u + j) % x;
            if (dist[nxt] > dist[u] + j) { dist[nxt] = dist[u] + j; pq.push({dist[nxt], nxt}); }
        }
    }
    long long ans = 0;
    for (int i = 1; i < x; i++) {
        if (dist[i] == LLONG_MAX) { cout << -1 << endl; return 0; }
        ans = max(ans, dist[i] - x);
    }
    cout << ans << endl;
    return 0;
}
