#include <bits/stdc++.h>

using namespace std;

const int MAXN = 300001;
const int MAXT = MAXN * 22;
int n, m, depth;

int head[MAXN];
int to[MAXN << 1];
int nxt[MAXN << 1];
int cntg = 0;

int root[MAXN];
int ls[MAXT];
int rs[MAXT];
long long sum[MAXT];
int cntt = 0;

int dep[MAXN];
int siz[MAXN];
int dfn[MAXN];
int cntd = 0;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

int build(int l, int r) {
    int rt = ++cntt;
    sum[rt] = 0LL;
    if (l < r) {
        int mid = (l + r) >> 1;
        ls[rt] = build(l, mid);
        rs[rt] = build(mid + 1, r);
    }
    return rt;
}

int add(int jobi, long long jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    sum[rt] = sum[i] + jobv;
    if (l < r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = add(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = add(jobi, jobv, mid + 1, r, rs[rt]);
        }
    }
    return rt;
}

long long query(int jobl, int jobr, int l, int r, int u, int v) {
    if (jobl <= l && r <= jobr) {
        return sum[v] - sum[u];
    }
    long long ans = 0;
    int mid = (l + r) >> 1;
    if (jobl <= mid) {
        ans += query(jobl, jobr, l, mid, ls[u], ls[v]);
    }
    if (jobr > mid) {
        ans += query(jobl, jobr, mid + 1, r, rs[u], rs[v]);
    }
    return ans;
}

void dfs1(int u, int f) {
    dep[u] = dep[f] + 1;
    depth = max(depth, dep[u]);
    siz[u] = 1;
    dfn[u] = ++cntd;
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        if (to[ei] != f) {
            dfs1(to[ei], u);
        }
    }
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        if (to[ei] != f) {
            siz[u] += siz[to[ei]];
        }
    }
}

void dfs2(int u, int f) {
    root[dfn[u]] = add(dep[u], (long long)siz[u] - 1, 1, depth, root[dfn[u] - 1]);
    for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
        if (to[ei] != f) {
            dfs2(to[ei], u);
        }
    }
}

void prepare() {
    depth = 0;
    dfs1(1, 0);
    root[0] = build(1, depth);
    dfs2(1, 0);
}

long long compute(int a, int k) {
    long long ans = (long long)(siz[a] - 1) * min(k, dep[a] - 1);
    ans += query(dep[a] + 1, dep[a] + k, 1, depth, root[dfn[a] - 1], root[dfn[a] + siz[a] - 1]);
    return ans;
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
    prepare();
    for(int i = 1, a, k; i <= m; i++) {
        cin >> a >> k;
        cout << compute(a, k) << "\n";
    }
    return 0;
}
