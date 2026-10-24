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

const int MAXN = 20001;
const int MAXM = 20001;
const int MAXT = 2000001;
const int MAXE = 5000001;
int n, m, cntt;
int v[MAXN];
int x[MAXM];
int y[MAXM];

int rak[MAXN];
Node arr[MAXN];

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

int rootOut1[MAXN];
int rootIn1[MAXN];
int rootOut2[MAXN];
int rootIn2[MAXN];
int ls[MAXT];
int rs[MAXT];
int curVersion;

int small(int num) {
    int l = 1, r = n, mid, ans = 0;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (arr[mid].v <= num) {
            ans = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return ans;
}

int big(int num) {
    int l = 1, r = n, mid, ans = n + 1;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (arr[mid].v >= num) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return n - ans + 1;
}

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

int addOut(int jobx, int jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(i, rt);
    if (l == r) {
        addEdge(jobx, rt);
    } else {
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            ls[rt] = addOut(jobx, jobv, l, mid, ls[rt]);
            addEdge(ls[rt], rt);
        } else {
            rs[rt] = addOut(jobx, jobv, mid + 1, r, rs[rt]);
            addEdge(rs[rt], rt);
        }
    }
    return rt;
}

int addIn(int jobx, int jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(rt, i);
    if (l == r) {
        addEdge(rt, jobx);
    } else {
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            ls[rt] = addIn(jobx, jobv, l, mid, ls[rt]);
            addEdge(rt, ls[rt]);
        } else {
            rs[rt] = addIn(jobx, jobv, mid + 1, r, rs[rt]);
            addEdge(rt, rs[rt]);
        }
    }
    return rt;
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

void add(int x, int otherx, int si, int bi) {
    curVersion++;
    rootOut1[curVersion] = addOut(x, si, 1, n, rootOut1[curVersion - 1]);
    rootIn1[curVersion] = addIn(otherx, si, 1, n, rootIn1[curVersion - 1]);
    rootOut2[curVersion] = addOut(otherx, bi, 1, n, rootOut2[curVersion - 1]);
    rootIn2[curVersion] = addIn(x, bi, 1, n, rootIn2[curVersion - 1]);
}

void link(int x, int otherx, int lowCnt, int hightCnt) {
    if (lowCnt > 0) {
        xToRange(x, 1, lowCnt, 1, n, rootIn1[curVersion]);
        rangeToX(1, lowCnt, otherx, 1, n, rootOut1[curVersion]);
    }
    if (hightCnt > 0) {
        xToRange(otherx, 1, hightCnt, 1, n, rootIn2[curVersion]);
        rangeToX(1, hightCnt, x, 1, n, rootOut2[curVersion]);
    }
}

int other(int x) {
    return x <= n ? x + n : x - n;
}

void buildGraph(int k) {
    cntt = n << 1;
    rootOut1[0] = buildOut(1, n);
    rootIn1[0] = buildIn(1, n);
    rootOut2[0] = buildOut(1, n);
    rootIn2[0] = buildIn(1, n);
    for (int i = 1; i <= m; i++) {
        addEdge(x[i], other(y[i]));
        addEdge(y[i], other(x[i]));
        addEdge(other(x[i]), y[i]);
        addEdge(other(y[i]), x[i]);
    }
    for (int i = 1; i <= n; i++) {
        link(i, other(i), small(v[i] - k), big(v[i] + k));
        add(i, other(i), rak[i], n - rak[i] + 1);
    }
}

void clear() {
    for (int i = 1; i <= cntt; i++) {
        head[i] = dfn[i] = belong[i] = 0;
    }
    cntt = cntg = cntd = top = sccCnt = curVersion = 0;
}

bool check(int k) {
    buildGraph(k);
    for (int i = 1; i <= (n << 1); i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1; i <= n; i++) {
        if (belong[i] == belong[i + n]) {
            check = false;
            break;
        }
    }
    clear();
    return check;
}

int compute() {
    for (int i = 1; i <= n; i++) {
        arr[i] = {v[i], i};
    }
    sort(arr + 1, arr + n + 1, NodeCmp);
    for (int i = 1; i <= n; i++) {
        rak[arr[i].i] = i;
    }
    int l = 0, r = arr[n].v, mid, ans = -1;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (check(mid)) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
    }
    for (int i = 1; i <= m; i++) {
        cin >> x[i] >> y[i];
    }
    int ans = compute();
    cout << ans << "\n";
    return 0;
}
