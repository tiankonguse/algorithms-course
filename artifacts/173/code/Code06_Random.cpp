#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXB = 401;
const int MAXV = 30001;
const int MAXP = 17;
int n, m, f, k;
int arr[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int dep[MAXN];
int stjump[MAXN][MAXP];

int markNum;
bool vis[MAXN];
int markNode[MAXN];
int kthMark[MAXN];
int up[MAXN];
bitset<MAXV> downSet[MAXB];

bitset<MAXV> ans;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u, int fa) {
    dep[u] = dep[fa] + 1;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = head[u]; e; e = nxt[e]) {
        if (to[e] != fa) {
            dfs(to[e], u);
        }
    }
}

int lca(int a, int b) {
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

void query(int x, int xylca) {
    while (kthMark[x] == 0 && x != xylca) {
        ans[arr[x]] = 1;
        x = stjump[x][0];
    }
    while (up[x] && dep[up[x]] > dep[xylca]) {
        ans |= downSet[kthMark[x]];
        x = up[x];
    }
    while (x != xylca) {
        ans[arr[x]] = 1;
        x = stjump[x][0];
    }
}

void updateAns(int x, int y) {
    int xylca = lca(x, y);
    query(x, xylca);
    query(y, xylca);
    ans[arr[xylca]] = 1;
}

void prepare() {
    dfs(1, 0);
    int len = (int)sqrt(n * 10);
    markNum = (n + len - 1) / len;
    for (int b = 1, pick; b <= markNum; b++) {
        do {
            pick = rand() % n + 1;
        } while (vis[pick]);
        vis[pick] = true;
        markNode[b] = pick;
        kthMark[pick] = b;
    }
    for (int b = 1, cur; b <= markNum; b++) {
        downSet[b][arr[markNode[b]]] = 1;
        cur = stjump[markNode[b]][0];
        while (cur != 0) {
            if (kthMark[cur] > 0) {
                up[markNode[b]] = cur;
                break;
            } else {
                downSet[b][arr[cur]] = 1;
                cur = stjump[cur][0];
            }
        }
    }	
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(0));
    cin >> n >> m >> f;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    prepare();
    for (int i = 1, lastAns = 0; i <= m; i++) {
        ans.reset();
        cin >> k;
        for (int j = 1, x, y; j <= k; j++) {
            cin >> x >> y;
            if (f) {
                x ^= lastAns;
                y ^= lastAns;
            }
            updateAns(x, y);
        }
        int ans1 = ans.count();
        int ans2 = MAXV;
        for (int i = 0; i < MAXV; i++) {
            if (ans[i] == 0) {
                ans2 = i;
                break;
            }
        }
        cout << ans1 << ' ' << ans2 << '\n';
        lastAns = ans1 + ans2;
    }
    return 0;
}
