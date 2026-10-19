#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXT = MAXN * 20;
int n;
int arr[MAXN];
int sorted[MAXN];
int cntv;

int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int cntg;

int root[MAXN];
int ls[MAXT];
int rs[MAXT];
int maxv[MAXT];
int addLazy[MAXT];
int cntt;

int kth(int num) {
    int left = 1, right = cntv, ret = 0;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (sorted[mid] <= num) {
            ret = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ret;
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void up(int i) {
    maxv[i] = max(maxv[ls[i]], maxv[rs[i]]);
}

void lazy(int i, int v) {
    if (i != 0) {
        maxv[i] += v;
        addLazy[i] += v;
    }
}

void down(int i) {
    if (addLazy[i] > 0) {
        lazy(ls[i], addLazy[i]);
        lazy(rs[i], addLazy[i]);
        addLazy[i] = 0;
    }
}

int update(int jobi, int jobv, int l, int r, int i) {
    int rt = i;
    if (rt == 0) {
        rt = ++cntt;
    }
    if (l == r) {
        maxv[rt] = max(maxv[rt], jobv);
    } else {
        down(rt);
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = update(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = update(jobi, jobv, mid + 1, r, rs[rt]);
        }
        up(rt);
    }
    return rt;
}

int query(int jobl, int jobr, int l, int r, int i) {
    if (i == 0) {
        return 0;
    }
    if (jobl <= l && r <= jobr) {
        return maxv[i];
    }
    down(i);
    int mid = (l + r) >> 1;
    int ans = 0;
    if (jobl <= mid) {
        ans = max(ans, query(jobl, jobr, l, mid, ls[i]));
    }
    if (jobr > mid) {
        ans = max(ans, query(jobl, jobr, mid + 1, r, rs[i]));
    }
    return ans;
}

int merge(int l, int r, int t1, int t2, int max1, int max2) {
    if (t1 == 0 || t2 == 0) {
        if (t1 != 0) {
            lazy(t1, max2);
        }
        if (t2 != 0) {
            lazy(t2, max1);
        }
        return t1 + t2;
    }
    if (l == r) {
        maxv[t1] += max(maxv[t2], max2);
    } else {
        down(t1);
        down(t2);
        int mid = (l + r) >> 1;
        ls[t1] = merge(l, mid, ls[t1], ls[t2], max(max1, maxv[rs[t1]]), max(max2, maxv[rs[t2]]));
        rs[t1] = merge(mid + 1, r, rs[t1], rs[t2], max1, max2);
        up(t1);
    }
    return t1;
}

void dp(int u) {
    int sum = 0;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        dp(v);
        sum += query(arr[u], cntv, 1, cntv, root[v]);
        root[u] = merge(1, cntv, root[u], root[v], 0, 0);
    }
    root[u] = update(arr[u], sum + 1, 1, cntv, root[u]);
}

void prepare() {
    for (int i = 1; i <= n; i++) {
        sorted[++cntv] = arr[i];
    }
    sort(sorted + 1, sorted + cntv + 1);
    int len = 1;
    for (int i = 2; i <= cntv; i++) {
        if (sorted[len] != sorted[i]) {
            sorted[++len] = sorted[i];
        }
    }
    cntv = len;
    for (int i = 1; i <= n; i++) {
        arr[i] = kth(arr[i]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 2, fa; i <= n; i++) {
        cin >> fa;
        addEdge(fa, i);
    }
    prepare();
    dp(1);
    cout << query(1, cntv, 1, cntv, root[1]) << '\n';
    return 0;
}
