#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> g[20];
long long pathCount(int s, int t, int visited) {
    if (s == t) return 1;
    long long c = 0;
    visited |= (1 << s);
    for (int v : g[s]) if (!((visited >> v) & 1)) c += pathCount(v, t, visited);
    return c;
}
int main() {
    int prob; cin >> prob;
    int t; cin >> t;
    while (t--) {
        cin >> n;
        for (int i = 0; i < 20; i++) g[i].clear();
        for (int i = 0; i < n; i++) {
            int u, v; cin >> u >> v;
            g[u].push_back(v); g[v].push_back(u);
        }
        long long ans = 0;
        for (int s = 1; s <= n; s++)
            for (int t2 = s + 1; t2 <= n; t2++)
                ans += pathCount(s, t2, 0);
        cout << ans << "\n";
    }
    return 0;
}
