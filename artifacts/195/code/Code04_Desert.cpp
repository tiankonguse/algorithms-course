#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXT = MAXN * 5;
const int MAXE = MAXN * 20;
const int LIMIT = 1000000000;
int n, s, m;

int head[MAXT];
int nxt[MAXE];
int to[MAXE];
int weight[MAXE];
int cntg;

int ls[MAXT];
int rs[MAXT];
int root;
int cntt;

int val[MAXT];
int atMost[MAXT];
int indegree[MAXT];
int que[MAXT];

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
    head[u] = cntg;
    indegree[v]++;
}

int build(int l, int r) {
    int rt;
    if (l == r) {
        rt = l;
    } else {
        rt = ++cntt;
        int mid = (l + r) >> 1;
        ls[rt] = build(l, mid);
        rs[rt] = build(mid + 1, r);
        addEdge(rt, ls[rt], 0);
        addEdge(rt, rs[rt], 0);
    }
    return rt;
}

void xToRange(int jobx, int jobl, int jobr, int jobw, int l, int r, int i) {
    if (jobl > jobr) {
        return;
    }
    if (jobl <= l && r <= jobr) {
        addEdge(jobx, i, jobw);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            xToRange(jobx, jobl, jobr, jobw, l, mid, ls[i]);
        }
        if (jobr > mid) {
            xToRange(jobx, jobl, jobr, jobw, mid + 1, r, rs[i]);
        }
    }
}

bool topo() {
    int qi = 1, qsiz = 0;
    for (int i = 1; i <= cntt; i++) {
        if (indegree[i] == 0) {
            que[++qsiz] = i;
        }
        atMost[i] = val[i] == 0 ? LIMIT : val[i];
    }
    while (qi <= qsiz) {
        int u = que[qi++];
        for (int e = head[u]; e > 0; e = nxt[e]) {
            int v = to[e];
            int w = weight[e];
            if (atMost[v] > atMost[u] + w) {
                atMost[v] = atMost[u] + w;
                if ((val[v] != 0 && atMost[v] < val[v]) || atMost[v] < 1) {
                    return false;
                }
            }
            if (--indegree[v] == 0) {
                que[++qsiz] = v;
            }
        }
    }
    return qsiz == cntt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> s >> m;
    cntt = n;
    root = build(1, n);
    for (int i = 1; i <= s; i++) {
        int x, v;
        cin >> x >> v;
        val[x] = v;
    }
    for (int i = 1; i <= m; i++) {
        int l, r, k;
        cin >> l >> r >> k;
        int vnode = ++cntt;
        for (int j = 1; j <= k; j++) {
            int x;
            cin >> x;
            addEdge(x, vnode, 0);
            xToRange(vnode, l, x - 1, -1, 1, n, root);
            l = x + 1;
        }
        xToRange(vnode, l, r, -1, 1, n, root);
    }
    bool check = topo();
    if (check) {
        cout << "TAK" << "\n";
        for (int i = 1; i <= n; i++) {
            cout << atMost[i] << " ";
        }
        cout << "\n";
    } else {
        cout << "NIE" << "\n";
    }
    return 0;
}
