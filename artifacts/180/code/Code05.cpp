#include <bits/stdc++.h>

using namespace std;

struct Node {
    int id, dist, time, virus;
    bool operator<(const Node &other) const {
        if (time != other.time) {
            return time > other.time;
        }
        return virus > other.virus;
    }
};

const int MAXN = 200001;
const int MAXP = 20;
int n, q, k, m;

int headg[MAXN];
int nextg[MAXN << 1];
int tog[MAXN << 1];
int cntg;

int headv[MAXN];
int nextv[MAXN << 1];
int tov[MAXN << 1];
int cntv;

int dep[MAXN];
int dfn[MAXN];
int stjump[MAXN][MAXP];
int cntd;

int start[MAXN];
int speed[MAXN];
int query[MAXN];

int arr[MAXN << 1];
int tmp[MAXN << 2];
int len;

priority_queue<Node> heap;
bool vis[MAXN];
int minTime[MAXN];
int findVirus[MAXN];

bool cmp(int x, int y) {
    return dfn[x] < dfn[y];
}

void addEdgeG(int u, int v) {
    nextg[++cntg] = headg[u];
    tog[cntg] = v;
    headg[u] = cntg;
}

void addEdgeV(int u, int v) {
    nextv[++cntv] = headv[u];
    tov[cntv] = v;
    headv[u] = cntv;
}

void dfs(int u, int fa) {
    dep[u] = dep[fa] + 1;
    dfn[u] = ++cntd;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = headg[u]; e; e = nextg[e]) {
        if (tog[e] != fa) {
            dfs(tog[e], u);
        }
    }
}

int getLca(int a, int b) {
    if (dep[a] < dep[b]) {
        swap(a, b);
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (dep[stjump[a][p]] >= dep[b]) {
            a = stjump[a][p];
        }
    }
    if (a == b) {
        return a;
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }
    return stjump[a][0];
}

int buildVirtualTree() {
    int tot = 0;
    for (int i = 1; i <= k; i++) {
        arr[++tot] = start[i];
    }
    for (int i = 1; i <= m; i++) {
        arr[++tot] = query[i];
    }
    sort(arr + 1, arr + tot + 1, cmp);
    len = 0;
    for (int i = 1; i < tot; i++) {
        tmp[++len] = arr[i];
        tmp[++len] = getLca(arr[i], arr[i + 1]);
    }
    tmp[++len] = arr[tot];
    sort(tmp + 1, tmp + len + 1, cmp);
    int unique = 1;
    for (int i = 2; i <= len; i++) {
        if (tmp[unique] != tmp[i]) {
            tmp[++unique] = tmp[i];
        }
    }
    cntv = 0;
    for (int i = 1; i <= unique; i++) {
        headv[tmp[i]] = 0;
    }
    for (int i = 1; i < unique; i++) {
        int lca = getLca(tmp[i], tmp[i + 1]);
        addEdgeV(lca, tmp[i + 1]);
        addEdgeV(tmp[i + 1], lca);
    }
    len = unique;
    return tmp[1];
}

void dijkstra() {
    for (int i = 1; i <= len; i++) {
        int u = tmp[i];
        minTime[u] = n + 1;
        findVirus[u] = k + 1;
        vis[u] = false;
    }
    for (int i = 1; i <= k; i++) {
        int s = start[i];
        minTime[s] = 0;
        findVirus[s] = i;
        heap.push(Node{s, 0, 0, i});
    }
    while (!heap.empty()) {
        Node cur = heap.top();
        heap.pop();
        int u = cur.id;
        int udist = cur.dist;
        int uvirus = cur.virus;
        if (!vis[u]) {
            vis[u] = true;
            for (int e = headv[u]; e; e = nextv[e]) {
                int v = tov[e];
                if (!vis[v]) {
                    int vdist = udist + abs(dep[u] - dep[v]);
                    int vtime = (vdist + speed[uvirus] - 1) / speed[uvirus];
                    if (vtime < minTime[v] || (vtime == minTime[v] && uvirus < findVirus[v])) {
                        minTime[v] = vtime;
                        findVirus[v] = uvirus;
                        heap.push(Node{v, vdist, vtime, uvirus});
                    }
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdgeG(u, v);
        addEdgeG(v, u);
    }
    dfs(1, 0);
    cin >> q;
    for (int t = 1; t <= q; t++) {
        cin >> k >> m;
        for (int i = 1, s, v; i <= k; i++) {
            cin >> start[i] >> speed[i];
        }
        for (int i = 1; i <= m; i++) {
            cin >> query[i];
        }
        buildVirtualTree();
        dijkstra();
        for (int i = 1; i <= m; i++) {
            cout << findVirus[query[i]] << ' ';
        }
        cout << '\n';
    }
    return 0;
}
