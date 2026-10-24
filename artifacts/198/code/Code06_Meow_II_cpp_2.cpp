#include <bits/stdc++.h>

using namespace std;

struct Node {
    int v, i;
};

bool NodeCmp(Node a, Node b) {
    if (a.v != b.v) {
        return a.v < b.v;
    }
    return a.i < b.i;
}

struct Group {
    int l, r, x;
};

bool GroupCmp(Group a, Group b) {
    return a.r < b.r;
}

const int MAXV = 50001;
const int MAXN = 200001;
const int MAXT = 10000001;
const int MAXE = 50000001;
int v, n, cntt;

Node vi[MAXN];
Group group[MAXN];
int cntp;

int head[MAXT];
int nxt[MAXE];
int to[MAXE];
int cntg;

int dfn[MAXT];
int low[MAXT];
int cntd;

int sta[MAXT];
int top;

int belong[MAXT];
int sccCnt;

int rootOut[MAXN];
int rootIn[MAXN];
int ls[MAXT];
int rs[MAXT];

int ans[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else {
            if (belong[v] == 0) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    if (dfn[u] == low[u]) {
        sccCnt++;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
        } while (pop != u);
    }
}

int buildOut(int l, int r) {
    int rt = ++cntt;
    if (l < r) {
        int mid = (l + r) >> 1;
        ls[rt] = buildOut(l, mid);
        rs[rt] = buildOut(mid + 1, r);
        addEdge(ls[rt], rt);
        addEdge(rs[rt], rt);
    }
    return rt;
}

int buildIn(int l, int r) {
    int rt = ++cntt;
    if (l < r) {
        int mid = (l + r) >> 1;
        ls[rt] = buildIn(l, mid);
        rs[rt] = buildIn(mid + 1, r);
        addEdge(rt, ls[rt]);
        addEdge(rt, rs[rt]);
    }
    return rt;
}

int addOut(int jobi, int jobx, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(i, rt);
    if (l == r) {
        addEdge(jobx, rt);
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = addOut(jobi, jobx, l, mid, ls[rt]);
            addEdge(ls[rt], rt);
        } else {
            rs[rt] = addOut(jobi, jobx, mid + 1, r, rs[rt]);
            addEdge(rs[rt], rt);
        }
    }
    return rt;
}

int addIn(int jobi, int jobx, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(rt, i);
    if (l == r) {
        addEdge(rt, jobx);
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = addIn(jobi, jobx, l, mid, ls[rt]);
            addEdge(rt, ls[rt]);
        } else {
            rs[rt] = addIn(jobi, jobx, mid + 1, r, rs[rt]);
            addEdge(rt, rs[rt]);
        }
    }
    return rt;
}

void xToRange(int jobx, int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        addEdge(jobx, i);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            xToRange(jobx, jobl, jobr, l, mid, ls[i]);
        }
        if (jobr > mid) {
            xToRange(jobx, jobl, jobr, mid + 1, r, rs[i]);
        }
    }
}

void rangeToX(int jobl, int jobr, int jobx, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        addEdge(i, jobx);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            rangeToX(jobl, jobr, jobx, l, mid, ls[i]);
        }
        if (jobr > mid) {
            rangeToX(jobl, jobr, jobx, mid + 1, r, rs[i]);
        }
    }
}

int other(int x) {
    return x <= v ? x + v : x - v;
}

void buildGraph() {
    sort(group + 1, group + cntp + 1, GroupCmp);
    cntt = v << 1;
    rootOut[0] = buildOut(1, n);
    rootIn[0] = buildIn(1, n);
    for (int i = 1; i <= cntp; i++) {
        int l = group[i].l;
        int r = group[i].r;
        int x = group[i].x;
        xToRange(x, l, r, 1, n, rootIn[i - 1]);
        rangeToX(l, r, other(x), 1, n, rootOut[i - 1]);
        rootIn[i] = addIn(l, other(x), 1, n, rootIn[i - 1]);
        rootOut[i] = addOut(l, x, 1, n, rootOut[i - 1]);
    }
}

bool compute() {
    sort(vi + 1, vi + n + 1, NodeCmp);
    for (int i = 1; i <= n; i += 4) {
        int x = vi[i].v;
        int a = vi[i].i;
        int b = vi[i + 1].i;
        int c = vi[i + 2].i;
        int d = vi[i + 3].i;
        group[++cntp] = { a, c, x };
        group[++cntp] = { b, d, x };
        group[++cntp] = { a, b, x + v };
        group[++cntp] = { c, d, x + v };
    }
    buildGraph();
    for (int i = 1; i <= v << 1; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1; i <= v; i++) {
        if (belong[i] == belong[i + v]) {
            check = false;
            break;
        }
    }
    if (check) {
        for (int i = 1; i <= n; i += 4) {
            int x = vi[i].v;
            int a = vi[i].i;
            int b = vi[i + 1].i;
            int c = vi[i + 2].i;
            int d = vi[i + 3].i;
            if (belong[x] < belong[x + v]) {
                ans[a] = 0;
                ans[b] = 0;
                ans[c] = 1;
                ans[d] = 1;
            } else {
                ans[a] = 0;
                ans[b] = 1;
                ans[c] = 0;
                ans[d] = 1;
            }
        }
    }
    return check;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> v;
    n = v << 2;
    for (int i = 1; i <= n; i++) {
        cin >> vi[i].v;
        vi[i].i = i;
    }
    bool check = compute();
    if (check) {
        cout << "Yes" << "\n";
        for (int i = 1; i <= n; i++) {
            cout << ans[i];
        }
    } else {
        cout << "No" << "\n";
    }
    return 0;
}
