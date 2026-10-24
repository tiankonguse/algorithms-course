#include <bits/stdc++.h>

using namespace std;

const int MAXN = 501;
const int MAXM = 4001;
int t, n, m;

int v[MAXN];
int c[MAXN];
int d[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

bool vis[MAXN];
int siz[MAXN];

int nodeArr[MAXN];
int endDfn[MAXN];
int cntd;

int val[MAXN];
int cost[MAXN];
int dp[MAXN + 1][MAXM];

void prepare() {
    cntg = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = 0;
        vis[i] = false;
    }
}

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

void dfs(int u, int fa) {
    nodeArr[++cntd] = u;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            dfs(v, u);
        }
    }
    endDfn[u] = cntd;
}

int calc(int u) {
    cntd = 0;
    dfs(u, 0);
    for (int i = cntd; i > 0; i--) {
        int cur = nodeArr[i];
        int cnt = d[cur] - 1;
        int num = 0;
        for (int k = 1; k <= cnt; k <<= 1) {
            val[++num] = v[cur] * k;
            cost[num] = c[cur] * k;
            cnt -= k;
        }
        if (cnt > 0) {
            val[++num] = v[cur] * cnt;
            cost[num] = c[cur] * cnt;
        }
        for (int j = m; j >= c[cur]; j--) {
            dp[i][j] = dp[i + 1][j - c[cur]] + v[cur];
        }
        for (int k = 1; k <= num; k++) {
            for (int j = m; j >= cost[k]; j--) {
                dp[i][j] = max(dp[i][j], dp[i][j - cost[k]] + val[k]);
            }
        }
        for (int j = 0; j <= m; j++) {
            dp[i][j] = max(dp[i][j], dp[endDfn[cur] + 1][j]);
        }
    }
    int ans = dp[1][m];
    for (int i = 1; i <= cntd; i++) {
        for (int j = 0; j <= m; j++) {
            dp[i][j] = 0;
        }
    }
    return ans;
}

int solve(int u) {
    vis[u] = true;
    int ans = calc(u);
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            ans = max(ans, solve(getCentroid(v, u)));
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> t;
    for (int k = 1; k <= t; k++) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            cin >> v[i];
        }
        for (int i = 1; i <= n; i++) {
            cin >> c[i];
        }
        for (int i = 1; i <= n; i++) {
            cin >> d[i];
        }
        prepare();
        for (int i = 1, u, v; i < n; i++) {
            cin >> u >> v;
            addEdge(u, v);
            addEdge(v, u);
        }
        cout << solve(getCentroid(1, 0)) << '\n';
    }
    return 0;
}
