#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXM = 200001;
const int MAXP = 20;
int n, m, q;
int a[MAXM];
int b[MAXM];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int ebccCnt;

int block[MAXN];
int dep[MAXN];
int stjump[MAXN][MAXP];

bool vis[MAXN];
int upCnt[MAXN];
int downCnt[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v, e);
            low[u] = min(low[u], low[v]);
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        ebccCnt++;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = ebccCnt;
        } while (pop != u);
    }
}

void condense() {
    cntg = 0;
    for (int i = 1; i <= ebccCnt; i++) {
        head[i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int ebcc1 = belong[a[i]];
        int ebcc2 = belong[b[i]];
        if (ebcc1 != ebcc2) {
            addEdge(ebcc1, ebcc2);
            addEdge(ebcc2, ebcc1);
        }
    }
}

void dfs(int u, int fa, int bid) {
    block[u] = bid;
    dep[u] = dep[fa] + 1;
    stjump[u][0] = fa;
    for (int p = 1; p < MAXP; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dfs(v, u, bid);
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

bool check(int u, int fa) {
    vis[u] = true;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            if (!check(v, u)) {
                return false;
            }
            upCnt[u] += upCnt[v];
            downCnt[u] += downCnt[v];
        }
    }
    return upCnt[u] == 0 || downCnt[u] == 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cntg = 1;
    cin >> n >> m >> q;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];
        addEdge(a[i], b[i]);
        addEdge(b[i], a[i]);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i, 0);
        }
    }
    condense();
    for (int i = 1, b = 0; i <= ebccCnt; i++) {
        if (block[i] == 0) {
            dfs(i, 0, ++b);
        }
    }
    bool ans = true;
    for (int i = 1, x, y, xylca; i <= q; i++) {
        cin >> x >> y;
        x = belong[x];
        y = belong[y];
        if (block[x] != block[y]) {
            ans = false;
            break;
        }
        xylca = getLca(x, y);
        upCnt[x]++;
        upCnt[xylca]--;
        downCnt[y]++;
        downCnt[xylca]--;
    }
    if (ans) {
        for (int i = 1; i <= ebccCnt; i++) {
            if (!vis[i] && !check(i, 0)) {
                ans = false;
                break;
            }
        }
    }
    cout << (ans ? "Yes" : "No") << "\n";
    return 0;
}
