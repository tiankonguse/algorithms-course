#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 500001;
const int INF = 1000000001;
int n, m;

int val[MAXN];
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
int sccMin[MAXN];
int sccMax[MAXN];
int sccCnt;

int premin[MAXN];
int dp[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else {
            if (belong[v] == 0) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    if (dfn[u] == low[u]) {
        sccCnt++;
        sccMin[sccCnt] = INF;
        sccMax[sccCnt] = -INF;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
            sccMin[sccCnt] = min(sccMin[sccCnt], val[pop]);
            sccMax[sccCnt] = max(sccMax[sccCnt], val[pop]);
        } while (pop != u);
    }
}

void condense() {
    cntg = 0;
    for (int i = 1; i <= sccCnt; i++) {
        head[i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int scc1 = belong[a[i]];
        int scc2 = belong[b[i]];
        if (scc1 > 0 && scc2 > 0 && scc1 != scc2) {
            addEdge(scc1, scc2);
        }
    }
}

int dpOnDAG() {
    for (int u = 1; u <= sccCnt; u++) {
        premin[u] = INF;
        dp[u] = -INF;
    }
    int s = belong[1];
    premin[s] = sccMin[s];
    dp[s] = sccMax[s] - sccMin[s];
    for (int u = sccCnt; u > 0; u--) {
        for (int e = head[u]; e > 0; e = nxt[e]) {
            int v = to[e];
            premin[v] = min(premin[v], min(premin[u], sccMin[v]));
            dp[v] = max(dp[v], max(dp[u], sccMax[v] - premin[v]));
        }
    }
    return dp[belong[n]];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> val[i];
    }
    for (int i = 1, op; i <= m; i++) {
        cin >> a[i] >> b[i] >> op;
        if (op == 1) {
            addEdge(a[i], b[i]);
        } else {
            addEdge(a[i], b[i]);
            addEdge(b[i], a[i]);
        }
    }
    tarjan(1);
    if (belong[n] == 0) {
        cout << 0 << "\n";
    } else {
        condense();
        int ans = dpOnDAG();
        cout << ans << "\n";
    }
    return 0;
}
