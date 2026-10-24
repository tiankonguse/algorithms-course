#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
const int MAXM = 20001;
const int MAXT = 1000001;
const int MAXE = 2000001;
int n, m, cntt;
int v[MAXN];
int x[MAXM];
int y[MAXM];

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
    return x <= n ? x + n : x - n;
}

void merge(int l, int mid, int r, int k) {
    int preOut = 0, curOut = 0, preIn = 0, curIn = 0;
    for (int p1 = l - 1, p2 = mid + 1; p2 <= r; p2++) {
        curOut = ++cntt;
        curIn = ++cntt;
        while (p1 + 1 <= mid && v[arr[p1 + 1]] <= v[arr[p2]] - k) {
            p1++;
            addEdge(arr[p1], curOut);
            addEdge(curIn, other(arr[p1]));
        }
        if (preOut > 0) {
            addEdge(preOut, curOut);
            addEdge(curIn, preIn);
        }
        addEdge(curOut, other(arr[p2]));
        addEdge(arr[p2], curIn);
        preOut = curOut;
        preIn = curIn;
    }
    preOut = curOut = preIn = curIn = 0;
    for (int p1 = mid + 1, p2 = r; p2 >= mid + 1; p2--) {
        curOut = ++cntt;
        curIn = ++cntt;
        while (p1 - 1 >= l && v[arr[p1 - 1]] >= v[arr[p2]] + k) {
            p1--;
            addEdge(other(arr[p1]), curOut);
            addEdge(curIn, arr[p1]);
        }
        if (preOut > 0) {
            addEdge(preOut, curOut);
            addEdge(curIn, preIn);
        }
        addEdge(curOut, arr[p2]);
        addEdge(other(arr[p2]), curIn);
        preOut = curOut;
        preIn = curIn;
    }
    int p1 = l, p2 = mid + 1, ti = 0;
    while (p1 <= mid && p2 <= r) {
        if (v[arr[p1]] <= v[arr[p2]]) {
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

void cdq(int l, int r, int k) {
    if (l == r) {
        return;
    }
    int mid = (l + r) / 2;
    cdq(l, mid, k);
    cdq(mid + 1, r, k);
    merge(l, mid, r, k);
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
        arr[i] = i;
    }
    cdq(1, n, k);
}

void clear() {
    for (int i = 1; i <= cntt; i++) {
        head[i] = dfn[i] = belong[i] = 0;
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
    int maxv = v[1];
    for (int i = 2; i <= n; i++) {
        maxv = max(maxv, v[i]);
    }
    int l = 0, r = maxv, mid, ans = -1;
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
