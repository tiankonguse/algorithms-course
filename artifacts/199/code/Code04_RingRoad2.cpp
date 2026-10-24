#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 100001;
int n;
double k;
int arr[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int dfn[MAXN];
int cntd;

int x, y, skip;

int dp[MAXN][2];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u) {
    dfn[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            dfs(v);
        } else if (dfn[u] < dfn[v]) {
            x = u;
            y = v;
            skip = (e + 1) >> 1;
        }
    }
}

void dpOnTree(int u, int fa) {
    dp[u][0] = 0;
    dp[u][1] = arr[u];
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa && (e + 1) >> 1 != skip) {
            dpOnTree(v, u);
            dp[u][0] += max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    }
}

ll compute() {
    x = y = 0;
    dfs(1);
    dpOnTree(x, 0);
    ll ans = dp[x][0];
    dpOnTree(y, 0);
    ans = max(ans, 1ll * dp[y][0]);
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }
    for (int i = 1, u, v; i <= n; i++) {
        scanf("%d%d", &u, &v);
        u++;
        v++;
        addEdge(u, v);
        addEdge(v, u);
    }
    scanf("%lf", &k);
    ll ans = compute();
    printf("%.1f\n", k * ans);
    return 0;
}
