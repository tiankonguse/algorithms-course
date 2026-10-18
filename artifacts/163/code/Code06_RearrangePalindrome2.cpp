#include <bits/stdc++.h>

using namespace std;

const int MAXN = 500001;
const int MAXV = 22;
int n;
int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int weight[MAXN];
int cnt = 0;
int siz[MAXN];
int dep[MAXN];
int eor[MAXN];
int son[MAXN];
int maxdep[1 << MAXV];
int ans[MAXN];

void addEdge(int u, int v, int w) {
    nxt[++cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt;
}

void dfs1(int u, int d, int x) {
    siz[u] = 1;
    dep[u] = d;
    eor[u] = x;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        dfs1(to[e], d + 1, x ^ (1 << weight[e]));
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        siz[u] += siz[v];
        if (son[u] == 0 || siz[son[u]] < siz[v]) {
            son[u] = v;
        }
    }
}

void effect(int u) {
    maxdep[eor[u]] = max(maxdep[eor[u]], dep[u]);
    for (int e = head[u]; e > 0; e = nxt[e]) {
        effect(to[e]);
    }
}

void cancel(int u) {
    maxdep[eor[u]] = 0;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        cancel(to[e]);
    }
}

void answerFromLight(int light, int u) {
    if (maxdep[eor[light]] != 0) {
        ans[u] = max(ans[u], maxdep[eor[light]] + dep[light] - dep[u] * 2);
    }
    for (int i = 0; i < MAXV; i++) {
        if (maxdep[eor[light] ^ (1 << i)] != 0) {
            ans[u] = max(ans[u], maxdep[eor[light] ^ (1 << i)] + dep[light] - dep[u] * 2);
        }
    }
    for (int e = head[light]; e > 0; e = nxt[e]) {
        answerFromLight(to[e], u);
    }
}

void dfs2(int u, int keep) {
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u]) {
            dfs2(v, 0);
        }
    }
    if (son[u] != 0) {
        dfs2(son[u], 1);
    }
    for (int e = head[u]; e > 0; e = nxt[e]) {
        ans[u] = max(ans[u], ans[to[e]]);
    }
    if (maxdep[eor[u]] != 0) {
        ans[u] = max(ans[u], maxdep[eor[u]] - dep[u]);
    }
    for (int i = 0; i < MAXV; i++) {
        if (maxdep[eor[u] ^ (1 << i)] != 0) {
            ans[u] = max(ans[u], maxdep[eor[u] ^ (1 << i)] - dep[u]);
        }
    }
    maxdep[eor[u]] = max(maxdep[eor[u]], dep[u]);
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u]) {
            answerFromLight(v, u);
            effect(v);
        }
    }
    if (keep == 0) {
        cancel(u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    int fth;
    int edg;
    char c;
    for (int i = 2; i <= n; i++) {
        cin >> fth;
        cin >> c;
        edg = c - 'a';
        addEdge(fth, i, edg);
    }
    dfs1(1, 1, 0);
    dfs2(1, 0);
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << " ";
    }
    cout << "\n";
    return 0;
}
