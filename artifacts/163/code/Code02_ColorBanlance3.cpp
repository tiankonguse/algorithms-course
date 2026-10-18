#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
int n;
int color[MAXN];
int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int cnt = 0;
int siz[MAXN];
int son[MAXN];
int colorCnt[MAXN];
int colorNum[MAXN];
int ans = 0;

void addEdge(int u, int v) {
    nxt[++cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt;
}

void dfs1(int u) {
    siz[u] = 1;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        dfs1(to[e]);
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
    colorCnt[color[u]]++;
    colorNum[colorCnt[color[u]] - 1]--;
    colorNum[colorCnt[color[u]]]++;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        effect(to[e]);
    }
}

void cancel(int u) {
    colorCnt[color[u]]--;
    colorNum[colorCnt[color[u]] + 1]--;
    colorNum[colorCnt[color[u]]]++;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        cancel(to[e]);
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
    colorCnt[color[u]]++;
    colorNum[colorCnt[color[u]] - 1]--;
    colorNum[colorCnt[color[u]]]++;
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != son[u]) {
            effect(v);
        }
    }
    if (colorCnt[color[u]] * colorNum[colorCnt[color[u]]] == siz[u]) {
        ans++;
    }
    if (keep == 0) {
        cancel(u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, father; i <= n; i++) {
        cin >> color[i] >> father;
        if (i != 1) {
            addEdge(father, i);
        }
    }
    dfs1(1);
    dfs2(1, 0);
    cout << ans << "\n";
    return 0;
}
