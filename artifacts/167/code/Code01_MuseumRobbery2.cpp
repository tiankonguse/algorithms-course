#include <bits/stdc++.h>

using namespace std;

const int MAXN = 40001;
const int MAXQ = 30001;
const int MAXK = 1001;
const int MAXT = 1000001;
const int DEEP = 20;
const long long BAS = 10000019LL;
const long long MOD = 1000000007LL;
int n, k, q;

int v[MAXN];
int w[MAXN];

int op[MAXQ];
int x[MAXQ];
int y[MAXQ];

int from[MAXN];
int to[MAXN];

int head[MAXQ << 2];
int nxt[MAXT];
int tov[MAXT];
int tow[MAXT];
int cnt = 0;

long long dp[MAXK];
long long backup[DEEP][MAXK];
long long ans[MAXQ];

void clone(long long* a, long long* b) {
    for (int i = 0; i <= k; i++) {
        a[i] = b[i];
    }
}

void addEdge(int i, int v, int w) {
    nxt[++cnt] = head[i];
    tov[cnt] = v;
    tow[cnt] = w;
    head[i] = cnt;
}

void add(int jobl, int jobr, int jobv, int jobw, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        addEdge(i, jobv, jobw);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            add(jobl, jobr, jobv, jobw, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, jobw, mid + 1, r, i << 1 | 1);
        }
    }
}

void dfs(int l, int r, int i, int dep) {
    clone(backup[dep], dp);
    for (int e = head[i]; e > 0; e = nxt[e]) {
        int v = tov[e];
        int w = tow[e];
        for (int j = k; j >= w; j--) {
            dp[j] = max(dp[j], dp[j - w] + v);
        }
    }
    if (l == r) {
        if (op[l] == 3) {
            long long ret = 0;
            long long base = 1;
            for (int j = 1; j <= k; j++) {
                ret = (ret + dp[j] * base) % MOD;
                base = (base * BAS) % MOD;
            }
            ans[l] = ret;
        }
    } else {
        int mid = (l + r) >> 1;
        dfs(l, mid, i << 1, dep + 1);
        dfs(mid + 1, r, i << 1 | 1, dep + 1);
    }
    clone(dp, backup[dep]);
}

void prepare() {
    for (int i = 1; i <= n; i++) {
        from[i] = 1;
        to[i] = q;
    }
    for (int i = 1; i <= q; i++) {
        if (op[i] == 1) {
            n++;
            v[n] = x[i];
            w[n] = y[i];
            from[n] = i;
            to[n] = q;
        } else if (op[i] == 2) {
            to[x[i]] = i - 1;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (from[i] <= to[i]) {
            add(from[i], to[i], v[i], w[i], 1, q, 1);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> v[i] >> w[i];
    }
    cin >> q;
    for (int i = 1; i <= q; i++) {
        cin >> op[i];
        if (op[i] == 1) {
            cin >> x[i] >> y[i];
        } else if (op[i] == 2) {
            cin >> x[i];
        }
    }
    prepare();
    dfs(1, q, 1, 1);
    for (int i = 1; i <= q; i++) {
        if (op[i] == 3) {
            cout << ans[i] << '\n';
        }
    }
    return 0;
}
