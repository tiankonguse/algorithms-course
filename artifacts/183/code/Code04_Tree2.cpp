#include <bits/stdc++.h>

using namespace std;

const int MAXN = 50001;
int n, k;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int weight[MAXN << 1];
int cntg;

bool vis[MAXN];
int siz[MAXN];

int disArr[MAXN];
int cnta;

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
    if (dis > k) {
        return;
    }
    disArr[++cnta] = dis;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa && !vis[v]) {
            dfs(v, u, dis + weight[e]);
        }
    }
}

long long calc(int u, int dis) {
    cnta = 0;
    dfs(u, 0, dis);
    long long ans = 0;
    sort(disArr + 1, disArr + cnta + 1);
    for (int l = 1, r = cnta; l <= r; ) {
        if (disArr[l] + disArr[r] <= k) {
            ans += r - l;
            l++;
        } else {
            r--;
        }
    }
    return ans;
}

long long solve(int u) {
    vis[u] = true;
    long long ans = calc(u, 0);
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            ans -= calc(v, weight[e]);
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
    cin >> k;
    cout << solve(getCentroid(1, 0)) << '\n';
    return 0;
}
