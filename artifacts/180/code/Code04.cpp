#include <bits/stdc++.h>

using namespace std;

const int MAXN = 300001;
const int MAXP = 20;
int n, q, k;

int headg[MAXN];
int nextg[MAXN << 1];
int tog[MAXN << 1];
int cntg;

int headv[MAXN];
int nextv[MAXN];
int tov[MAXN];
int cntv;

int dep[MAXN];
int siz[MAXN];
int dfn[MAXN];
int stjump[MAXN][MAXP];
int cntd;

int order[MAXN];
int arr[MAXN];
bool isKey[MAXN];
int tmp[MAXN << 1];

int manager[MAXN];
int dist[MAXN];
int ans[MAXN];

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
    siz[u] = 1;
    dfn[u] = ++cntd;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = headg[u]; e; e = nextg[e]) {
        int v = tog[e];
        if (v != fa) {
            dfs(v, u);
            siz[u] += siz[v];
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
    sort(arr + 1, arr + k + 1, cmp);
    int len = 0;
    tmp[++len] = 1;
    for (int i = 1; i < k; i++) {
        tmp[++len] = arr[i];
        tmp[++len] = getLca(arr[i], arr[i + 1]);
    }
    tmp[++len] = arr[k];
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
        addEdgeV(getLca(tmp[i], tmp[i + 1]), tmp[i + 1]);
    }
    return tmp[1];
}

void dp1(int u) {
    dist[u] = 1000000001;
    for (int e = headv[u]; e; e = nextv[e]) {
        int v = tov[e];
        dp1(v);
        int w = dep[v] - dep[u];
        if (dist[v] + w < dist[u] || (dist[v] + w == dist[u] && manager[v] < manager[u])) {
            dist[u] = dist[v] + w;
            manager[u] = manager[v];
        }
    }
    if (isKey[u]) {
        dist[u] = 0;
        manager[u] = u;
    }
}

void dp2(int u) {
    for (int e = headv[u]; e; e = nextv[e]) {
        int v = tov[e];
        int w = dep[v] - dep[u];
        if (dist[u] + w < dist[v] || (dist[u] + w == dist[v] && manager[u] < manager[v])) {
            dist[v] = dist[u] + w;
            manager[v] = manager[u];
        }
        dp2(v);
    }
}

void amend(int u, int v) {
    if (manager[u] == manager[v]) {
        return;
    }
    int x = v;
    for (int p = MAXP - 1; p >= 0; p--) {
        int jump = stjump[x][p];
        if (dep[u] < dep[jump]) {
            int tou = dep[jump] - dep[u] + dist[u];
            int tov = dep[v] - dep[jump] + dist[v];
            if (tov < tou || (tov == tou && manager[v] < manager[u])) {
                x = jump;
            }
        }
    }
    int delta = siz[x] - siz[v];
    ans[manager[u]] -= delta;
    ans[manager[v]] += delta;
}

void dp3(int u) {
    ans[manager[u]] += siz[u];
    for (int e = headv[u]; e; e = nextv[e]) {
        int v = tov[e];
        amend(u, v);
        ans[manager[u]] -= siz[v];
        dp3(v);
    }
}

void compute() {
    for (int i = 1; i <= k; i++) {
        arr[i] = order[i];
    }
    for (int i = 1; i <= k; i++) {
        isKey[arr[i]] = true;
        ans[arr[i]] = 0;
    }
    int tree = buildVirtualTree();
    dp1(tree);
    dp2(tree);
    dp3(tree);
    for (int i = 1; i <= k; i++) {
        isKey[arr[i]] = false;
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
        cin >> k;
        for (int i = 1; i <= k; i++) {
            cin >> order[i];
        }
        compute();
        for (int i = 1; i <= k; i++) {
            cout << ans[order[i]] << ' ';
        }
        cout << '\n';
    }
    return 0;
}
