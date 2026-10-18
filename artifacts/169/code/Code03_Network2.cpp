#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 200001;
const int MAXP = 20;
const int INF = 1000000001;
int n, m;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg = 0;

int fa[MAXN];
int dep[MAXN];
int siz[MAXN];
int dfn[MAXN];
int stjump[MAXN][MAXP];
int cntd = 0;

int tree[MAXN];

int eid[MAXM];
int op[MAXM];
int x[MAXM];
int y[MAXM];
int v[MAXM];
int cntq = 0;

int lset[MAXM];
int rset[MAXM];

int ans[MAXM];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f] + 1;
    siz[u] = 1;
    dfn[u] = ++cntd;
    stjump[u][0] = f;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = head[u]; e != 0; e = nxt[e]) {
        if (to[e] != f) {
            dfs(to[e], u);
        }
    }
    for (int e = head[u]; e != 0; e = nxt[e]) {
        if (to[e] != f) {
            siz[u] += siz[to[e]];
        }
    }
}

int lca(int a, int b) {
    if (dep[a] < dep[b]) {
        int tmp = a;
        a = b;
        b = tmp;
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

int lowbit(int i) {
    return i & -i;
}

void add(int i, int v) {
    while (i <= n) {
        tree[i] += v;
        i += lowbit(i);
    }
}

int query(int i) {
    int ret = 0;
    while (i > 0) {
        ret += tree[i];
        i -= lowbit(i);
    }
    return ret;
}

void pathAdd(int x, int y, int v) {
    int xylca = lca(x, y);
    int lcafa = fa[xylca];
    add(dfn[x], v);
    add(dfn[y], v);
    add(dfn[xylca], -v);
    if (lcafa != 0) {
        add(dfn[lcafa], -v);
    }
}

int pointQuery(int x) {
    return query(dfn[x] + siz[x] - 1) - query(dfn[x] - 1);
}

void compute(int el, int er, int vl, int vr) {
    if (el > er) {
        return;
    }
    if (vl == vr) {
        for (int i = el; i <= er; i++) {
            int id = eid[i];
            if (op[id] == 2) {
                ans[y[id]] = vl;
            }
        }
    } else {
        int mid = (vl + vr) / 2;
        int lsiz = 0, rsiz = 0, request = 0;
        for (int i = el; i <= er; i++) {
            int id = eid[i];
            if (op[id] == 0) {
                if (v[id] <= mid) {
                    lset[++lsiz] = id;
                } else {
                    pathAdd(x[id], y[id], 1);
                    request++;
                    rset[++rsiz] = id;
                }
            } else if (op[id] == 1) {
                if (v[id] <= mid) {
                    lset[++lsiz] = id;
                } else {
                    pathAdd(x[id], y[id], -1);
                    request--;
                    rset[++rsiz] = id;
                }
            } else {
                if (pointQuery(x[id]) == request) {
                    lset[++lsiz] = id;
                } else {
                    rset[++rsiz] = id;
                }
            }
        }
        for (int i = 1; i <= lsiz; i++) {
            eid[el + i - 1] = lset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            eid[el + lsiz + i - 1] = rset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            int id = rset[i];
            if (op[id] == 0 && v[id] > mid) {
                pathAdd(x[id], y[id], -1);
            }
            if (op[id] == 1 && v[id] > mid) {
                pathAdd(x[id], y[id], 1);
            }
        }
        compute(el, el + lsiz - 1, vl, mid);
        compute(el + lsiz, er, mid + 1, vr);
    }
}

void prepare() {
    dfs(1, 0);
    for (int i = 1; i <= m; i++) {
        if (op[i] == 1) {
            int pre = x[i];
            x[i] = x[pre];
            y[i] = y[pre];
            v[i] = v[pre];
        }
        if (op[i] == 2) {
            y[i] = ++cntq;
        }
    }
    for (int i = 1; i <= m; i++) {
        eid[i] = i;
    }
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
    for (int i = 1; i <= m; i++) {
        cin >> op[i] >> x[i];
        if (op[i] == 0) {
            cin >> y[i] >> v[i];
        }
    }
    prepare();
    compute(1, m, 0, INF);
    for (int i = 1; i <= cntq; i++) {
        if (ans[i] == 0) {
            cout << -1 << '\n';
        } else {
            cout << ans[i] << '\n';
        }
    }
    return 0;
}
