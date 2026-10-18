#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
int n, m;
int arr[MAXN][3];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg = 0;

int fa[MAXN];
int dep[MAXN];
int siz[MAXN];
int son[MAXN];
int top[MAXN];
int dfn[MAXN];
int cntd = 0;

int sumv[MAXN << 2];
int maxv[MAXN << 2];
int minv[MAXN << 2];
bool negativeTag[MAXN << 2];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs1(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f] + 1;
    siz[u] = 1;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != f) {
            dfs1(v, u);
        }
    }
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != f) {
            siz[u] += siz[v];
            if (son[u] == 0 || siz[son[u]] < siz[v]) {
                son[u] = v;
            }
        }
    }
}

void dfs2(int u, int t) {
    top[u] = t;
    dfn[u] = ++cntd;
    if (son[u] == 0) {
        return;
    }
    dfs2(son[u], t);
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa[u] && v != son[u]) {
            dfs2(v, v);
        }
    }
}

void up(int i) {
    int l = i << 1, r = i << 1 | 1;
    sumv[i] = sumv[l] + sumv[r];
    maxv[i] = max(maxv[l], maxv[r]);
    minv[i] = min(minv[l], minv[r]);
}

void lazy(int i) {
    sumv[i] = -sumv[i];
    int tmp = maxv[i];
    maxv[i] = -minv[i];
    minv[i] = -tmp;
    negativeTag[i] = !negativeTag[i];
}

void down(int i) {
    if (negativeTag[i]) {
        lazy(i << 1);
        lazy(i << 1 | 1);
        negativeTag[i] = false;
    }
}

void update(int jobi, int jobv, int l, int r, int i) {
    if (l == r) {
        sumv[i] = maxv[i] = minv[i] = jobv;
    } else {
        down(i);
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            update(jobi, jobv, l, mid, i << 1);
        } else {
            update(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i);
    }
}

void negative(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        lazy(i);
    } else {
        down(i);
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
        	negative(jobl, jobr, l, mid, i << 1);
        }
        if (jobr > mid) {
        	negative(jobl, jobr, mid + 1, r, i << 1 | 1);
        }
        up(i);
    }
}

int querySum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return sumv[i];
    }
    down(i);
    int mid = (l + r) >> 1;
    int ans = 0;
    if (jobl <= mid) {
        ans += querySum(jobl, jobr, l, mid, i << 1);
    }
    if (jobr > mid) {
        ans += querySum(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans;
}

int queryMax(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return maxv[i];
    }
    down(i);
    int mid = (l + r) >> 1;
    int ans = INT_MIN;
    if (jobl <= mid) {
        ans = max(ans, queryMax(jobl, jobr, l, mid, i << 1));
    }
    if (jobr > mid) {
        ans = max(ans, queryMax(jobl, jobr, mid + 1, r, i << 1 | 1));
    }
    return ans;
}

int queryMin(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return minv[i];
    }
    down(i);
    int mid = (l + r) >> 1;
    int ans = INT_MAX;
    if (jobl <= mid) {
        ans = min(ans, queryMin(jobl, jobr, l, mid, i << 1));
    }
    if (jobr > mid) {
        ans = min(ans, queryMin(jobl, jobr, mid + 1, r, i << 1 | 1));
    }
    return ans;
}

void edgeUpdate(int ei, int val) {
    int x = arr[ei][0];
    int y = arr[ei][1];
    int downx = max(dfn[x], dfn[y]);
    update(downx, val, 1, n, 1);
}

void pathNegative(int x, int y) {
    while (top[x] != top[y]) {
        if (dep[top[x]] <= dep[top[y]]) {
        	negative(dfn[top[y]], dfn[y], 1, n, 1);
            y = fa[top[y]];
        } else {
        	negative(dfn[top[x]], dfn[x], 1, n, 1);
            x = fa[top[x]];
        }
    }
    negative(min(dfn[x], dfn[y]) + 1, max(dfn[x], dfn[y]), 1, n, 1);
}

int pathSum(int x, int y) {
    int ans = 0;
    while (top[x] != top[y]) {
        if (dep[top[x]] <= dep[top[y]]) {
            ans += querySum(dfn[top[y]], dfn[y], 1, n, 1);
            y = fa[top[y]];
        } else {
            ans += querySum(dfn[top[x]], dfn[x], 1, n, 1);
            x = fa[top[x]];
        }
    }
    ans += querySum(min(dfn[x], dfn[y]) + 1, max(dfn[x], dfn[y]), 1, n, 1);
    return ans;
}

int pathMax(int x, int y) {
    int ans = INT_MIN;
    while (top[x] != top[y]) {
        if (dep[top[x]] <= dep[top[y]]) {
            ans = max(ans, queryMax(dfn[top[y]], dfn[y], 1, n, 1));
            y = fa[top[y]];
        } else {
            ans = max(ans, queryMax(dfn[top[x]], dfn[x], 1, n, 1));
            x = fa[top[x]];
        }
    }
    ans = max(ans, queryMax(min(dfn[x], dfn[y]) + 1, max(dfn[x], dfn[y]), 1, n, 1));
    return ans;
}

int pathMin(int x, int y) {
    int ans = INT_MAX;
    while (top[x] != top[y]) {
        if (dep[top[x]] <= dep[top[y]]) {
            ans = min(ans, queryMin(dfn[top[y]], dfn[y], 1, n, 1));
            y = fa[top[y]];
        } else {
            ans = min(ans, queryMin(dfn[top[x]], dfn[x], 1, n, 1));
            x = fa[top[x]];
        }
    }
    ans = min(ans, queryMin(min(dfn[x], dfn[y]) + 1, max(dfn[x], dfn[y]), 1, n, 1));
    return ans;
}

void prepare() {
    for (int i = 1; i < n; i++) {
        addEdge(arr[i][0], arr[i][1]);
        addEdge(arr[i][1], arr[i][0]);
    }
    dfs1(1, 0);
    dfs2(1, 1);
    for (int i = 1; i < n; i++) {
        edgeUpdate(i, arr[i][2]);
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for(int i = 1; i < n; i++){
        cin >> arr[i][0] >> arr[i][1] >> arr[i][2];
        arr[i][0]++;
        arr[i][1]++;
    }
    prepare();
    cin >> m;
    string op;
    for(int i = 1, x, y; i <= m; i++){
        string op;
        cin >> op;
        if(op == "C"){
            cin >> x >> y;
            edgeUpdate(x, y);
        } else {
            cin >> x >> y;
            x++;
            y++;
            if(op == "N"){
            	pathNegative(x, y);
            } else if(op == "SUM"){
                cout << pathSum(x, y) << "\n";
            } else if(op == "MAX"){
                cout << pathMax(x, y) << "\n";
            } else {
                cout << pathMin(x, y) << "\n";
            }
        }
    }
    return 0;
}
