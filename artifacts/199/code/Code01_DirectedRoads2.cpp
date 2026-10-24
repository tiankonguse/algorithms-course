#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 200001;
const int MOD = 1000000007;
int n, cntb;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int dfn[MAXN];
int cntd;

int from[MAXN];
int all[MAXN];
int cycle[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

ll power(ll x, ll p) {
    ll ans = 1;
    while (p > 0) {
        if ((p & 1) == 1) {
            ans = (ans * x) % MOD;
        }
        x = (x * x) % MOD;
        p >>= 1;
    }
    return ans;
}

void dfs(int u) {
    dfn[u] = ++cntd;
    all[cntb]++;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            from[v] = u;
            dfs(v);
        } else if (dfn[u] < dfn[v]) {
            cycle[cntb]++;
            for (int i = v; i != u; i = from[i]) {
                cycle[cntb]++;
            }
        }
    }
}

ll compute() {
    ll ans = 1;
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            cntb++;
            dfs(i);
            ll a = power(2, all[cntb]);
            ll b = power(2, all[cntb] - cycle[cntb] + 1);
            ans = ans * ((a - b + MOD) % MOD) % MOD;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, x; i <= n; i++) {
        cin >> x;
        addEdge(i, x);
        addEdge(x, i);
    }
    ll ans = compute();
    cout << ans << "\n";
    return 0;
}
