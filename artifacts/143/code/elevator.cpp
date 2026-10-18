#include <bits/stdc++.h>
using namespace std;
// 跳楼机 P3403：同余最短路(dijkstra)，模数取第一个数 x
long long h;
int x, y, z;
long long dist[100001];
bool vis[100001];
int main() {
    cin >> h >> x >> y >> z;
    h -= 1;
    for (int i = 0; i < x; i++) { dist[i] = LLONG_MAX; vis[i] = false; }
    dist[0] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, 0});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int u = top.second;
        if (vis[u]) continue;
        vis[u] = true;
        int nxt = (u + y) % x;
        if (dist[nxt] > dist[u] + y) { dist[nxt] = dist[u] + y; pq.push({dist[nxt], nxt}); }
        nxt = (u + z) % x;
        if (dist[nxt] > dist[u] + z) { dist[nxt] = dist[u] + z; pq.push({dist[nxt], nxt}); }
    }
    long long ans = 0;
    for (int i = 0; i < x; i++) {
        if (dist[i] <= h) ans += (h - dist[i]) / x + 1;
    }
    cout << ans << endl;
    return 0;
}
