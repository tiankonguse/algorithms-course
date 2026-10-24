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
const int MAXT = 1000001;
const int MAXE = 2000001;
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

int outTree1[MAXN];
int inTree1[MAXN];
int outTree2[MAXN];
int inTree2[MAXN];

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

int lowbit(int i) {
    return i & -i;
}

void addOut(int outTree[], int i, int x) {
    while (i <= n) {
        int preo = outTree[i];
        int curo = ++cntt;
        if (preo > 0) {
            addEdge(preo, curo);
        }
        addEdge(x, curo);
        outTree[i] = curo;
        i += lowbit(i);
    }
}

void addIn(int inTree[], int i, int x) {
    while (i <= n) {
        int prei = inTree[i];
        int curi = ++cntt;
        if (prei > 0) {
            addEdge(curi, prei);
        }
        addEdge(curi, x);
        inTree[i] = curi;
        i += lowbit(i);
    }
}

void rangeToX(int outTree[], int i, int x) {
    while (i > 0) {
        if (outTree[i] > 0) {
            addEdge(outTree[i], x);
        }
        i -= lowbit(i);
    }
}

void xToRange(int inTree[], int x, int i) {
    while (i > 0) {
        if (inTree[i] > 0) {
            addEdge(x, inTree[i]);
        }
        i -= lowbit(i);
    }
}

void add(int x, int otherx, int si, int bi) {
    addOut(outTree1, si, x);
    addIn(inTree1, si, otherx);
    addOut(outTree2, bi, otherx);
    addIn(inTree2, bi, x);
}

void link(int x, int otherx, int lowCnt, int highCnt) {
    xToRange(inTree1, x, lowCnt);
    rangeToX(outTree1, lowCnt, otherx);
    xToRange(inTree2, otherx, highCnt);
    rangeToX(outTree2, highCnt, x);
}

int other(int x) {
    return x <= n ? x + n : x - n;
}

void buildGraph(int k) {
    cntt = n << 1;
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
    for (int i = 1; i <= n; i++) {
        inTree1[i] = outTree1[i] = inTree2[i] = outTree2[i] = 0;
    }
    cntt = cntg = cntd = top = sccCnt = 0;
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
