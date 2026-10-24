#include <bits/stdc++.h>

using namespace std;

const int MAXN = 301;
const int MAXM = 601;
int n, m;
int w[MAXN];
int v[MAXN];
int depend[MAXN];

int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int wsum[MAXN];
int vsum[MAXN];
int sccCnt;

int indegree[MAXN];

int siz[MAXN];
int weight[MAXN];
int value[MAXN];
int dfnCnt;
int dp[MAXN][MAXM];

void addEdge(int a, int b) {
    nxt[++cntg] = head[a];
    to[cntg] = b;
    head[a] = cntg;
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
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
            wsum[sccCnt] += w[pop];
            vsum[sccCnt] += v[pop];
        } while (pop != u);
    }
}

void condense() {
    cntg = 0;
    for (int i = 0; i <= sccCnt; i++) {
        head[i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        if (depend[i] > 0) {
            int scc1 = belong[depend[i]];
            int scc2 = belong[i];
            if (scc1 != scc2) {
                indegree[scc2]++;
                addEdge(scc1, scc2);
            }
        }
    }
    for (int i = 1; i <= sccCnt; i++) {
        if (indegree[i] == 0) {
            addEdge(0, i);
        }
    }
}

int dfs(int u) {
    int i = ++dfnCnt;
    siz[i] = 1;
    weight[i] = wsum[u];
    value[i] = vsum[u];
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        siz[i] += dfs(v);
    }
    return siz[i];
}

int knapsackOnTree() {
    dfs(0);
    for (int i = dfnCnt; i >= 2; i--) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = dp[i + siz[i]][j];
            if (j - weight[i] >= 0) {
                dp[i][j] = max(dp[i][j], value[i] + dp[i + 1][j - weight[i]]);
            }
        }
    }
    return dp[2][m];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> depend[i];
        if (depend[i] > 0) {
            addEdge(depend[i], i);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    condense();
    int ans = knapsackOnTree();
    cout << ans << "\n";
    return 0;
}
