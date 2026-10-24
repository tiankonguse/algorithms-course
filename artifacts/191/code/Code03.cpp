#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 1000001;
int n, m, k, l;
int acnt[MAXN];
int bcnt[MAXN];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int ans1[MAXN];
int ans2[MAXN];
int cnta;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v, e);
            low[u] = min(low[u], low[v]);
            if (low[v] > dfn[u]) {
                if (acnt[v] == 0 || acnt[v] == k || bcnt[v] == 0 || bcnt[v] == l) {
                    cnta++;
                    ans1[cnta] = v;
                    ans2[cnta] = u;
                }
            }
            acnt[u] += acnt[v];
            bcnt[u] += bcnt[v];
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cntg = 1;
    cin >> n >> m >> k >> l;
    for (int i = 1, x; i <= k; i++) {
        cin >> x;
        acnt[x] = 1;
    }
    for (int i = 1, x; i <= l; i++) {
        cin >> x;
        bcnt[x] = 1;
    }
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    tarjan(1, 0);
    cout << cnta << "\n";
    for (int i = 1; i <= cnta; i++) {
        cout << ans1[i] << " " << ans2[i] << "\n";
    }
    return 0;
}
