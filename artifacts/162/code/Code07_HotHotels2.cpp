#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
int n;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int fa[MAXN];
int son[MAXN];
int len[MAXN];
int cntd;

int fid[MAXN];
int gid[MAXN];
long long f[MAXN];
long long g[MAXN << 1];
long long ans;

void setf(int u, int i, long long v) {
    f[fid[u] + i] = v;
}

long long getf(int u, int i) {
    return f[fid[u] + i];
}

void setg(int u, int i, long long v) {
    g[gid[u] + i] = v;
}

long long getg(int u, int i) {
    return g[gid[u] + i];
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs1(int u, int f) {
    fa[u] = f;
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            dfs1(v, u);
        }
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            if (son[u] == 0 || len[son[u]] < len[v]) {
                son[u] = v;
            }
        }
    }
    len[u] = len[son[u]] + 1;
}

void dfs2(int u, int top) {
    fid[u] = cntd++;
    if (son[u] == 0) {
        gid[u] = fid[top] * 2;
        return;
    }
    dfs2(son[u], top);
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u] && v != fa[u]) {
            dfs2(v, v);
        }
    }
    gid[u] = gid[son[u]] + 1;
}

void dfs3(int u) {
    setf(u, 0, 1);
    if (son[u] == 0) {
        return;
    }
    dfs3(son[u]);
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u] && v != fa[u]) {
            dfs3(v);
        }
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u] && v != fa[u]) {
            for (int i = 0; i <= len[v]; i++) {
                if (i < len[u] && i - 1 >= 0) {
                    ans += getg(u, i) * getf(v, i - 1);
                }
                if (i > 0 && i + 1 < len[v]) {
                    ans += getf(u, i) * getg(v, i + 1);
                }
            }
            for (int i = 0; i <= len[v]; i++) {
                if (i + 1 < len[v]) {
                    setg(u, i, getg(u, i) + getg(v, i + 1));
                }
                if (i - 1 >= 0) {
                    setg(u, i, getg(u, i) + getf(u, i) * getf(v, i - 1));
                }
            }
            for (int i = 0; i <= len[v]; i++) {
                if (i - 1 >= 0) {
                    setf(u, i, getf(u, i) + getf(v, i - 1));
                }
            }
        }
    }
    ans += getg(u, 0);
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
    dfs2(1, 1);
    dfs3(1);
    cout << ans << "\n";
    return 0;
}
