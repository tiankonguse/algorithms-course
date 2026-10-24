#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
int n, m;
int a[MAXN];
int b[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int weight[MAXN << 1];
int cntg;

bool vis[MAXN];
int siz[MAXN];

int tree[MAXN];
int dist[MAXN];

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

void dfs(int u, int fa, int d, int t) {
    tree[u] = t;
    dist[u] = d;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dfs(v, u, d + weight[e], t);
        }
    }
}

int compute() {
    int ans = 1000000001;
    int u = getCentroid(1, 0);
    while (!vis[u]) {
        vis[u] = true;
        tree[u] = u;
        dist[u] = 0;
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to[e];
            int w = weight[e];
            dfs(v, u, w, v);
        }
        int maxDist = 0, son = 0;
        for (int i = 1; i <= m; i++) {
            int curDist = dist[a[i]] + dist[b[i]];
            int t1 = tree[a[i]];
            int t2 = tree[b[i]];
            if (maxDist < curDist) {
                maxDist = curDist;
                son = (t1 == t2) ? t1 : 0;
            } else if (maxDist == curDist && (t1 != t2 || t1 != son)) {
                son = 0;
            }
        }
        ans = min(ans, maxDist);
        if (son == 0) {
            break;
        }
        u = getCentroid(son, u);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];
    }
    cout << compute() << '\n';
    return 0;
}
