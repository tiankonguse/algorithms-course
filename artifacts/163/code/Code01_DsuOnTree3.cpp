#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
int n, m;
int arr[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cnt = 0;

int fa[MAXN];
int siz[MAXN];
int son[MAXN];

int colorCnt[MAXN];
int ans[MAXN];
int diffColors = 0;

void addEdge(int u, int v) {
    nxt[++cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt;
}

void dfs1(int u, int f) {
    fa[u] = f;
    siz[u] = 1;
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            dfs1(v, u);
        }
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            siz[u] += siz[v];
            if (son[u] == 0 || siz[son[u]] < siz[v]) {
                son[u] = v;
            }
        }
    }
}

void effect(int u) {
    if (++colorCnt[arr[u]] == 1) {
        diffColors++;
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa[u]) {
            effect(v);
        }
    }
}

void cancel(int u) {
    colorCnt[arr[u]] = 0;
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa[u]) {
            cancel(v);
        }
    }
}

void dfs2(int u, int keep) {
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa[u] && v != son[u]) {
            dfs2(v, 0);
        }
    }
    if (son[u] != 0) {
        dfs2(son[u], 1);
    }
    if (++colorCnt[arr[u]] == 1) {
        diffColors++;
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa[u] && v != son[u]) {
            effect(v);
        }
    }
    ans[u] = diffColors;
    if (keep == 0) {
        diffColors = 0;
        cancel(u);
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
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    dfs1(1, 0);
    dfs2(1, 0);
    cin >> m;
    for (int i = 1, cur; i <= m; i++) {
        cin >> cur;
        cout << ans[cur] << "\n";
    }
    return 0;
}
