#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3001;
const int LIMIT = 10000;
const double NA = -1e9;
const double sml = 1e-6;

int k, n;
int head[MAXN], nxt[MAXN], to[MAXN], edgeCnt;
int cost[MAXN], strength[MAXN];
int dfn[MAXN], dfnCnt;
double value[MAXN];
int sz[MAXN];
double dp[MAXN][MAXN];

void addEdge(int u, int v) {
    nxt[edgeCnt] = head[u];
    to[edgeCnt] = v;
    head[u] = edgeCnt++;
}

int dfs(int u) {
    int i = ++dfnCnt;
    dfn[u] = i;
    sz[i] = 1;
    for (int e = head[u]; e != 0; e = nxt[e]) {
        int v = to[e];
        sz[i] += dfs(v);
    }
    return sz[i];
}

bool check(double x) {
    for (int i = 0; i <= n; i++) value[dfn[i]] = (double)strength[i] - x * cost[i];
    for (int j = 1; j <= k; j++) dp[dfnCnt + 1][j] = NA;
    for (int i = dfnCnt; i >= 2; i--) {
        for (int j = 1; j <= k; j++) {
            dp[i][j] = max(dp[i + sz[i]][j], value[i] + dp[i + 1][j - 1]);
        }
    }
    return dp[2][k] >= 0;
}

int main() {
    scanf("%d%d", &k, &n);
    edgeCnt = 1;
    dfnCnt = 0;
    fill(head + 1, head + n + 1, 0);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &cost[i], &strength[i]);
        int p;
        scanf("%d", &p);
        addEdge(p, i);
    }
    dfs(0);
    double l = 0, r = LIMIT, ans = 0;
    while (l < r && r - l >= sml) {
        double x = (l + r) / 2;
        if (check(x)) {
            ans = x;
            l = x + sml;
        } else {
            r = x - sml;
        }
    }
    printf("%.3f\n", ans);
    return 0;
}
