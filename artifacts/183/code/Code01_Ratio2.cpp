#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
int n;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int weight[MAXN << 1];
int cntg;

bool vis[MAXN];
int siz[MAXN];

int cur[3];
int all[3];

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

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

void dfs(int u, int fa, int dis) {
    cur[dis % 3]++;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            dfs(v, u, dis + weight[e]);
        }
    }
}

int calc(int u) {
    int ans = 1;
    all[0] = all[1] = all[2] = 0;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        int w = weight[e];
        if (!vis[v]) {
            cur[0] = cur[1] = cur[2] = 0;
            dfs(v, u, w);
            ans += all[0] * cur[0] * 2 + all[1] * cur[2] * 2 + all[2] * cur[1] * 2 + cur[0] * 2;
            all[0] += cur[0];
            all[1] += cur[1];
            all[2] += cur[2];
        }
    }
    return ans;
}

int solve(int u) {
    int ans = 0;
    vis[u] = true;
    ans += calc(u);
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            ans += solve(getCentroid(v, u));
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    int centroid = getCentroid(1, 0);
    int a = solve(centroid);
    int b = n * n;
    int c = gcd(a, b);
    a /= c;
    b /= c;
    cout << a << "/" << b << '\n';
    return 0;
}
