#include <bits/stdc++.h>

using namespace std;

const int MAXN = 80001;
const int MAXM = 200001;
const int INF = 1000000001;
int n, m, s;

int a[MAXM];
int b[MAXM];
int init[MAXM];
int recover[MAXM];

int head[MAXN];
int nxt[MAXM];
int to[MAXM];
int weight[MAXM];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int sum[MAXN];
int sccCnt;

int dp[MAXN];

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
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
        int popv;
        do {
            popv = sta[top--];
            belong[popv] = sccCnt;
        } while (popv != u);
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
        if (scc1 > 0 && scc2 > 0) {
            int val = init[i];
            int rec = recover[i];
            if (scc1 == scc2) {
                while (val > 0) {
                    sum[scc1] += val;
                    val = val * rec / 10;
                }
            } else {
                addEdge(scc1, scc2, val);
            }
        }
    }
}

int dpOnDAG() {
    for (int u = 1; u <= sccCnt; u++) {
        dp[u] = -INF;
    }
    dp[belong[s]] = sum[belong[s]];
    for (int u = sccCnt; u > 0; u--) {
        for (int e = head[u]; e > 0; e = nxt[e]) {
            int v = to[e];
            int w = weight[e];
            dp[v] = max(dp[v], dp[u] + w + sum[v]);
        }
    }
    int ans = 0;
    for (int u = 1; u <= sccCnt; u++) {
        ans = max(ans, dp[u]);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    double rec;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i] >> init[i];
        cin >> rec;
        recover[i] = (int)(rec * 10);
        addEdge(a[i], b[i], 0);
    }
    cin >> s;
    tarjan(s);
    condense();
    int ans = dpOnDAG();
    cout << ans << "\n";
    return 0;
}
