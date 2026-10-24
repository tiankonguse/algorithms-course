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
const int MAXT = 5000001;
const int MAXE = 20000001;
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

int arr[MAXN];
int tmp[MAXN];

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

int other(int x) {
    return x <= v ? x + v : x - v;
}

void merge(int l, int mid, int r) {
    int preOut = 0, curOut = 0, preIn = 0, curIn = 0;
    for (int p1 = mid + 1, p2 = r; p2 >= mid + 1; p2--) {
        curOut = ++cntt;
        curIn = ++cntt;
        while (p1 - 1 >= l && group[arr[p1 - 1]].l >= group[arr[p2]].l) {
            p1--;
            addEdge(group[arr[p1]].x, curOut);
            addEdge(curIn, other(group[arr[p1]].x));
        }
        if (preOut > 0) {
            addEdge(preOut, curOut);
            addEdge(curIn, preIn);
        }
        addEdge(curOut, other(group[arr[p2]].x));
        addEdge(group[arr[p2]].x, curIn);
        preOut = curOut;
        preIn = curIn;
    }
    int p1 = l, p2 = mid + 1, ti = 0;
    while (p1 <= mid && p2 <= r) {
        if (group[arr[p1]].l <= group[arr[p2]].l) {
            tmp[++ti] = arr[p1++];
        } else {
            tmp[++ti] = arr[p2++];
        }
    }
    while (p1 <= mid) {
        tmp[++ti] = arr[p1++];
    }
    while (p2 <= r) {
        tmp[++ti] = arr[p2++];
    }
    for (int i = l, j = 1; i <= r; i++, j++) {
        arr[i] = tmp[j];
    }
}

void cdq(int l, int r) {
    if (l == r) {
        return;
    }
    int mid = (l + r) / 2;
    cdq(l, mid);
    cdq(mid + 1, r);
    merge(l, mid, r);
}

void buildGraph() {
    sort(group + 1, group + cntp + 1, GroupCmp);
    cntt = v << 1;
    for (int i = 1; i <= cntp; i++) {
        arr[i] = i;
    }
    cdq(1, cntp);
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
