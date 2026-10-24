#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const long long BASE = 499;
int t, n, m;

char val[MAXN];
char str[MAXN];
int a[MAXN];
int b[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

long long pre[MAXN];
long long suf[MAXN];

bool vis[MAXN];
int siz[MAXN];

int deep[MAXN];
long long curp[MAXN];
long long curs[MAXN];
long long allp[MAXN];
long long alls[MAXN];

long long ans;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void getSize(int u, int fa) {
    siz[u] = 1;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            getSize(v, u);
            siz[u] += siz[v];
        }
    }
}

int getCentroid(int u, int fa) {
    getSize(u, fa);
    int half = siz[u] >> 1;
    bool find = false;
    while (!find) {
        find = true;
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to[e];
            if (v != fa && !vis[v] && siz[v] > half) {
                fa = u;
                u = v;
                find = false;
                break;
            }
        }
    }
    return u;
}

void dfs(int u, int fa, int dep, long long hash) {
    deep[u] = dep;
    hash = hash * BASE + (val[u] - 'A' + 1);
    if (hash == pre[dep]) {
        curp[(dep - 1) % m + 1]++;
        ans += alls[m - (dep - 1) % m];
    }
    if (hash == suf[dep]) {
        curs[(dep - 1) % m + 1]++;
        ans += allp[m - (dep - 1) % m];
    }
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            dfs(v, u, dep + 1, hash);
            deep[u] = max(deep[u], deep[v]);
        }
    }
}

void calc(int u) {
    int maxDep = 0;
    allp[1] = alls[1] = 1;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            dfs(v, u, 2, 1LL * (val[u] - 'A' + 1));
            int curDep = min(deep[v], m);
            for (int i = 1; i <= curDep; i++) {
                allp[i] += curp[i];
                alls[i] += curs[i];
                curp[i] = curs[i] = 0;
            }
            maxDep = max(maxDep, curDep);
        }
    }
    for (int i = 1; i <= maxDep; i++) {
        allp[i] = alls[i] = 0;
    }
}

void solve(int u) {
    vis[u] = true;
    calc(u);
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            solve(getCentroid(v, u));
        }
    }
}

void prepare() {
    cntg = 0;
    ans = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = 0;
        vis[i] = false;
    }
    for (int i = 1; i < n; i++) {
        addEdge(a[i], b[i]);
        addEdge(b[i], a[i]);
    }
    long long tmp = 1;
    for (int i = 1; i <= n; i++) {
        pre[i] = pre[i - 1] + tmp * (str[(i - 1) % m + 1] - 'A' + 1);
        suf[i] = suf[i - 1] + tmp * (str[m - (i - 1) % m] - 'A' + 1);
        tmp = tmp * BASE;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> t;
    for (int k = 1; k <= t; k++) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            cin >> val[i];
        }
        for (int i = 1; i < n; i++) {
            cin >> a[i] >> b[i];
        }
        for (int i = 1; i <= m; i++) {
            cin >> str[i];
        }
        prepare();
        solve(getCentroid(1, 0));
        cout << ans << '\n';
    }
    return 0;
}
