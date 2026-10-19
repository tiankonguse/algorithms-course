#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000001;
const int MAXP = 20;
const long long INF = 1LL << 60;
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
int dfn[MAXN];
int stjump[MAXN][MAXP];
int cntd;

int arr[MAXN];
bool isKey[MAXN];

int tmp[MAXN << 1];
int stk[MAXN];

int siz[MAXN];
long long sum[MAXN];
long long near[MAXN];
long long far[MAXN];
long long costSum, costMin, costMax;

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
        stjump[u][p] = stjump[ stjump[u][p - 1] ][p - 1];
    }
    for (int e = headg[u]; e; e = nextg[e]) {
        int v = tog[e];
        if (v != fa) dfs(v, u);
    }
}

int getLca(int a, int b) {
    if (dep[a] < dep[b]) {
        swap(a, b);
    }
    for (int p = MAXP - 1; p >= 0; p--) {
        if (dep[ stjump[a][p] ] >= dep[b]) {
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

int buildVirtualTree1() {
    sort(arr + 1, arr + k + 1, cmp);
    int len = 0;
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

int buildVirtualTree2() {
    sort(arr + 1, arr + k + 1, cmp);
    cntv = 0;
    headv[arr[1]] = 0;
    int top = 0;
    stk[++top] = arr[1];
    for (int i = 2; i <= k; i++) {
        int x = arr[i];
        int y = stk[top];
        int lca = getLca(x, y);
        while (top > 1 && dfn[stk[top - 1]] >= dfn[lca]) {
            addEdgeV(stk[top - 1], stk[top]);
            top--;
        }
        if (lca != stk[top]) {
            headv[lca] = 0;
            addEdgeV(lca, stk[top]);
            stk[top] = lca;
        }
        headv[x] = 0;
        stk[++top] = x;
    }
    while (top > 1) {
        addEdgeV(stk[top - 1], stk[top]);
        top--;
    }
    return stk[1];
}

void dp(int u) {
    siz[u] = isKey[u] ? 1 : 0;
    sum[u] = 0;
    if (isKey[u]) {
        near[u] = far[u] = 0;
    } else {
        near[u] = INF;
        far[u] = -INF;
    }
    for (int e = headv[u]; e; e = nextv[e]) {
        dp(tov[e]);
    }
    for (int e = headv[u]; e; e = nextv[e]) {
        int v = tov[e];
        long long len = (long long)dep[v] - dep[u];
        costSum += (sum[u] + 1LL * siz[u] * len) * siz[v] + sum[v] * siz[u];
        siz[u] += siz[v];
        sum[u] += sum[v] + len * siz[v];
        costMin = min(costMin, near[u] + near[v] + len);
        costMax = max(costMax, far[u] + far[v] + len);
        near[u] = min(near[u], near[v] + len);
        far[u] = max(far[u], far[v] + len);
    }
}

void compute() {
    for (int i = 1; i <= k; i++) {
        isKey[arr[i]] = true;
    }
    int tree = buildVirtualTree1();
    // int tree = buildVirtualTree2();
    costSum = 0;
    costMin = INF;
    costMax = -INF;
    dp(tree);
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
            cin >> arr[i];
        }
        compute();
        cout << costSum << ' ' << costMin << ' ' << costMax << '\n';
    }
    return 0;
}
