#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000001;
int n;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg = 0;

int len[MAXN];
int son[MAXN];
int dfn[MAXN];
int cntd = 0;

int dp[MAXN];
int ansx[MAXN];

void setdp(int u, int i, int v) {
    dp[dfn[u] + i] = v;
}

int getdp(int u, int i) {
    return dp[dfn[u] + i];
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs1(int u, int fa) {
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa) {
            dfs1(v, u);
        }
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa) {
            if (son[u] == 0 || len[son[u]] < len[v]) {
                son[u] = v;
            }
        }
    }
    len[u] = len[son[u]] + 1;
}

void dfs2(int u, int fa) {
    dfn[u] = ++cntd;
    setdp(u, 0, 1);
    ansx[u] = 0;
    if (son[u] == 0) {
        return;
    }
    dfs2(son[u], u);
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa && v != son[u]) {
            dfs2(v, u);
        }
    }
    ansx[u] = ansx[son[u]] + 1;
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa && v != son[u]) {
            for (int i = 1; i <= len[v]; i++) {
                setdp(u, i, getdp(u, i) + getdp(v, i - 1));
                if (getdp(u, i) > getdp(u, ansx[u]) || (getdp(u, i) == getdp(u, ansx[u]) && i < ansx[u])) {
                    ansx[u] = i;
                }
            }
        }
    }
    if (getdp(u, ansx[u]) == 1) {
        ansx[u] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs1(1, 0);
    dfs2(1, 0);
    for (int i = 1; i <= n; i++) {
        cout << ansx[i] << "\n";
    }
    return 0;
}
