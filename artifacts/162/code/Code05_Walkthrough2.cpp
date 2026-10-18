#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
int n, k;
int arr[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cnt = 0;

int fa[MAXN];
int son[MAXN];
int top[MAXN];
long long money[MAXN];

long long sorted[MAXN];

void addEdge(int u, int v) {
    nxt[++cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt;
}

void dfs1(int u, int f) {
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            dfs1(v, u);
        }
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            if (son[u] == 0 || money[son[u]] < money[v]) {
                son[u] = v;
            }
        }
    }
    fa[u] = f;
    money[u] = money[son[u]] + arr[u];
}

void dfs2(int u, int t) {
    top[u] = t;
    if (son[u] == 0) {
        return;
    }
    dfs2(son[u], t);
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa[u] && v != son[u]) {
            dfs2(v, v);
        }
    }
}

long long compute() {
    int len = 0;
    for (int i = 1; i <= n; i++) {
        if (top[i] == i) {
        	sorted[++len] = money[i];
        }
    }
    sort(sorted + 1, sorted + len + 1);
    long long ans = 0;
    for (int i = 1, j = len; i <= k; i++, j--) {
        ans += sorted[j];
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs1(1, 0);
    dfs2(1, 1);
    cout << compute() << "\n";
    return 0;
}
