#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, t, lca, id;
};

struct Update {
    int pos, val;
};

const int MAXN = 100001;
const int MAXP = 20;
int n, m, q;
int v[MAXN];
int w[MAXN];
int c[MAXN];

int head[MAXN];
int to[MAXN << 1];
int nxt[MAXN << 1];
int cntg;

Query query[MAXN];
Update update[MAXN];
int cntq, cntu;

int dep[MAXN];
int seg[MAXN << 1];
int st[MAXN];
int ed[MAXN];
int stjump[MAXN][MAXP];
int cntd;

int bi[MAXN << 1];
bool vis[MAXN];
int cnt[MAXN];
long long happy;
long long ans[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u, int fa) {
    dep[u] = dep[fa] + 1;
    seg[++cntd] = u;
    st[u] = cntd;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = head[u], v; e > 0; e = nxt[e]) {
        v = to[e];
        if (v != fa) {
            dfs(v, u);
        }
    }
    seg[++cntd] = u;
    ed[u] = cntd;
}

int lca(int a, int b) {
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

bool QueryCmp(Query &a, Query &b) {
    if (bi[a.l] != bi[b.l]) {
        return bi[a.l] < bi[b.l];
    }
    if (bi[a.r] != bi[b.r]) {
        return bi[a.r] < bi[b.r];
    }
    return a.t < b.t;
}

void invert(int node) {
    int candy = c[node];
    if (vis[node]) {
        happy -= 1LL * v[candy] * w[cnt[candy]--];
    } else {
        happy += 1LL * v[candy] * w[++cnt[candy]];
    }
    vis[node] = !vis[node];
}

void moveTime(int tim) {
    int pos = update[tim].pos;
    int oldVal = c[pos];
    int newVal = update[tim].val;
    if (vis[pos]) {
        invert(pos);
        c[pos] = newVal;
        update[tim].val = oldVal;
        invert(pos);
    } else {
        c[pos] = newVal;
        update[tim].val = oldVal;
    }
}

void compute() {
    int winl = 1, winr = 0, wint = 0;
    for (int i = 1; i <= cntq; i++) {
        int jobl = query[i].l;
        int jobr = query[i].r;
        int jobt = query[i].t;
        int lca = query[i].lca;
        int id = query[i].id;
        while (winl > jobl) {
            invert(seg[--winl]);
        }
        while (winr < jobr) {
            invert(seg[++winr]);
        }
        while (winl < jobl) {
            invert(seg[winl++]);
        }
        while (winr > jobr) {
            invert(seg[winr--]);
        }
        while (wint < jobt) {
            moveTime(++wint);
        }
        while (wint > jobt) {
            moveTime(wint--);
        }
        if (lca > 0) {
            invert(lca);
        }
        ans[id] = happy;
        if (lca > 0) {
            invert(lca);
        }
    }
}

void prapare() {
    int blen = max(1, (int)pow(cntd, 2.0 / 3.0));
    for (int i = 1; i <= cntd; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    sort(query + 1, query + cntq + 1, QueryCmp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> q;
    for (int i = 1; i <= m; i++) {
        cin >> v[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1; i <= n; i++) {
        cin >> c[i];
    }
    dfs(1, 0);
    for (int i = 1, op, x, y; i <= q; i++) {
        cin >> op >> x >> y;
        if (op == 0) {
            cntu++;
            update[cntu].pos = x;
            update[cntu].val = y;
        } else {
            if (st[x] > st[y]) {
                swap(x, y);
            }
            int xylca = lca(x, y);
            if (x == xylca) {
                query[++cntq] = {st[x], st[y], cntu, 0, cntq};
            } else {
                query[++cntq] = {ed[x], st[y], cntu, xylca, cntq};
            }
        }
    }
    prapare();
    compute();
    for (int i = 1; i <= cntq; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
