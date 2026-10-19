#include <bits/stdc++.h>

using namespace std;

struct Node {
    int dis, edge;
};

const int MAXN = 200001;
const int MAXK = 1000001;
const int INF = 1000000001;
int n, k;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int weight[MAXN << 1];
int cntg;

bool vis[MAXN];
int siz[MAXN];

Node cur[MAXN];
int cntc;

int allDis[MAXN];
int cnta;

int dp[MAXK];

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
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

void dfs(int u, int fa, int dis, int edge) {
    if (dis > k) {
        return;
    }
    cur[++cntc] = { dis, edge };
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            dfs(v, u, dis + weight[e], edge + 1);
        }
    }
}

int calc(int u) {
    int ans = INF;
    cnta = 0;
    dp[0] = 0;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            cntc = 0;
            dfs(v, u, weight[e], 1);
            for (int i = 1; i <= cntc; i++) {
                ans = min(ans, dp[k - cur[i].dis] + cur[i].edge);
            }
            for (int i = 1; i <= cntc; i++) {
                allDis[++cnta] = cur[i].dis;
                dp[cur[i].dis] = min(dp[cur[i].dis], cur[i].edge);
            }
        }
    }
    for (int i = 1; i <= cnta; i++) {
        dp[allDis[i]] = INF;
    }
    return ans;
}

int solve(int u) {
    vis[u] = true;
    int ans = calc(u);
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            ans = min(ans, solve(getCentroid(v, u)));
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        u++;
        v++;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    fill(dp, dp + MAXK, INF);
    int ans = solve(getCentroid(1, 0));
    if (ans == INF) {
        ans = -1;
    }
    cout << ans << '\n';
    return 0;
}
