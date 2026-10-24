#include <bits/stdc++.h>
using namespace std;
int n, m;
long long d[20][20];
long long raw[20][20];
int eu[20], ev[20], ew[20];
int main() {
    int prob; cin >> prob;
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) raw[i][j] = (i == j ? 0 : 1e18);
    for (int i = 0; i < n; i++) {
        int u, v, w; cin >> u >> v >> w;
        eu[i] = u; ev[i] = v; ew[i] = w;
        raw[u][v] = w; raw[v][u] = w;
    }
    auto floyd = [&]() {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) d[i][j] = raw[i][j];
        for (int k = 1; k <= n; k++)
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    };
    floyd();
    for (int i = 0; i < m; i++) {
        int op, x, y; cin >> op >> x >> y;
        if (op == 1) {
            raw[eu[x - 1]][ev[x - 1]] = y;
            raw[ev[x - 1]][eu[x - 1]] = y;
            floyd();
        } else {
            cout << d[x][y] << "\n";
        }
    }
    return 0;
}
