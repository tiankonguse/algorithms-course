#include <bits/stdc++.h>
using namespace std;
int n;
int w[20];
vector<int> g[20];
int main() {
    int prob; cin >> prob;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> w[i];
    for (int i = 0; i < n; i++) {
        int u, v; cin >> u >> v;
        u++; v++;
        g[u].push_back(v); g[v].push_back(u);
    }
    double k; cin >> k;
    long long best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        bool ok = true;
        for (int i = 1; i <= n && ok; i++)
            if ((mask >> (i - 1)) & 1)
                for (int v : g[i])
                    if ((mask >> (v - 1)) & 1) { ok = false; break; }
        if (!ok) continue;
        long long s = 0;
        for (int i = 1; i <= n; i++) if ((mask >> (i - 1)) & 1) s += w[i];
        best = max(best, s);
    }
    printf("%.1f\n", k * best);
    return 0;
}
