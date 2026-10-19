#include <bits/stdc++.h>

using namespace std;

const int MAXN = 300001;
const int MAXT = MAXN * 50;
const int MAXP = 20;
int n, m;
int w[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int dep[MAXN];
int stjump[MAXN][MAXP];
int rtUp[MAXN];
int rtDown[MAXN];
int ls[MAXT];
int rs[MAXT];
int sum[MAXT];
int cntt;

int ans[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u, int fa) {
    dep[u] = dep[fa] + 1;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dfs(v, u);
        }
    }
}

int getLca(int a, int b) {
    if (dep[a] < dep[b]) {
        swap(a, b);
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (dep[stjump[a][p]] >= dep[b]) {
            a = stjump[a][p];
        }
    }
    if (a == b) {
        return a;
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }
    return stjump[a][0];
}

void up(int i) {
    sum[i] = sum[ls[i]] + sum[rs[i]];
}

int add(int jobi, int jobv, int l, int r, int i) {
    int rt = i;
    if (rt == 0) {
        rt = ++cntt;
    }
    if (l == r) {
        sum[rt] += jobv;
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = add(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = add(jobi, jobv, mid + 1, r, rs[rt]);
        }
        up(rt);
    }
    return rt;
}

int merge(int l, int r, int t1, int t2) {
    if (t1 == 0 || t2 == 0) {
        return t1 + t2;
    }
    if (l == r) {
        sum[t1] += sum[t2];
    } else {
        int mid = (l + r) >> 1;
        ls[t1] = merge(l, mid, ls[t1], ls[t2]);
        rs[t1] = merge(mid + 1, r, rs[t1], rs[t2]);
        up(t1);
    }
    return t1;
}

int query(int jobi, int l, int r, int i) {
    if (jobi < l || jobi > r || i == 0) {
        return 0;
    }
    if (l == r) {
        return sum[i];
    }
    int mid = (l + r) >> 1;
    if (jobi <= mid) {
        return query(jobi, l, mid, ls[i]);
    } else {
        return query(jobi, mid + 1, r, rs[i]);
    }
}

void calc(int u, int fa) {
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            calc(v, u);
            rtUp[u] = merge(1, n, rtUp[u], rtUp[v]);
            rtDown[u] = merge(-n, n, rtDown[u], rtDown[v]);
        }
    }
    ans[u] = query(dep[u] + w[u], 1, n, rtUp[u]) + query(dep[u] - w[u], -n, n, rtDown[u]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }
    dfs(1, 0);
    for (int i = 1, x, y; i <= m; i++) {
        cin >> x >> y;
        int lca = getLca(x, y);
        int lcafa = stjump[lca][0];
        rtUp[x] = add(dep[x], 1, 1, n, rtUp[x]);
        rtUp[lcafa] = add(dep[x], -1, 1, n, rtUp[lcafa]);
        rtDown[y] = add(2 * dep[lca] - dep[x], 1, -n, n, rtDown[y]);
        rtDown[lca] = add(2 * dep[lca] - dep[x], -1, -n, n, rtDown[lca]);
    }
    calc(1, 0);
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << ' ';
    }
    return 0;
}
