#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 200001;
int t, n;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int dfn[MAXN];
int cntd;

int from[MAXN];
bool cycle[MAXN];

ll siz[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u) {
    dfn[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            from[v] = u;
            dfs(v);
        } else if (dfn[u] < dfn[v]) {
            cycle[u] = true;
            for (int i = v; i != u; i = from[i]) {
                cycle[i] = true;
            }
        }
    }
}

void dpOnTree(int u, int fa) {
    siz[u] = 1;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !cycle[v]) {
            dpOnTree(v, u);
            siz[u] += siz[v];
        }
    }
}

ll compute() {
    dfs(1);
    ll ans = 0;
    for (int i = 1; i <= n; i++) {
        if (cycle[i]) {
            dpOnTree(i, 0);
            ans += siz[i] * (siz[i] - 1) / 2 + siz[i] * (n - siz[i]);
        }
    }
    return ans;
}

void prepare() {
    cntg = cntd = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = dfn[i] = 0;
        cycle[i] = false;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> t;
    for (int c = 1; c <= t; c++) {
        cin >> n;
        prepare();
        for (int i = 1, u, v; i <= n; i++) {
            cin >> u >> v;
            addEdge(u, v);
            addEdge(v, u);
        }
        ll ans = compute();
        cout << ans << "\n";
    }
    return 0;
}
