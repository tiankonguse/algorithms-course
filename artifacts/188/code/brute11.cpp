// 题11 暴力：状压 Dijkstra（标记边<=3、n<=6）
#include <bits/stdc++.h>
using namespace std;
int n, m, s;
vector<pair<int,int>> marks;
int markId[7][7];
const int INF = 1e9;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> m >> s;
    memset(markId, -1, sizeof(markId));
    marks.resize(m);
    for (int i = 0; i < m; i++) {
        cin >> marks[i].first >> marks[i].second;
        markId[marks[i].first][marks[i].second] = i;
        markId[marks[i].second][marks[i].first] = i;
    }
    // dp[mask][v]
    int K = 1 << m;
    vector<vector<int>> dp(K, vector<int>(n + 1, INF));
    dp[0][s] = 0;
    // Dijkstra on (mask, v)
    using St = tuple<int,int,int>; // d, mask, v
    priority_queue<St, vector<St>, greater<St>> pq;
    pq.push({0, 0, s});
    while (!pq.empty()) {
        auto [d, mask, v] = pq.top(); pq.pop();
        if (d > dp[mask][v]) continue;
        for (int u = 1; u <= n; u++) {
            int nd = d + abs(v - u);
            int nm = mask;
            int mi = markId[v][u];
            if (mi != -1) nm |= (1 << mi);
            if (nd < dp[nm][u]) {
                dp[nm][u] = nd;
                pq.push({nd, nm, u});
            }
        }
    }
    int FULL = K - 1;
    for (int i = 1; i <= n; i++) {
        int best = dp[FULL][i];
        if (i > 1) cout << " ";
        cout << best;
    }
    cout << "\n";
    return 0;
}
