#include <bits/stdc++.h>

using namespace std;

const int MAXN = 3001;
const int MAXM = 8001;
const int INF = 1000000001;
int n, p, m;

int cost[MAXN];
int a[MAXM];
int b[MAXM];

int head[MAXN];
int nxt[MAXM];
int to[MAXM];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int minVal[MAXN];
int sccCnt;

int indegree[MAXN];

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
        minVal[sccCnt] = INF;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
            minVal[sccCnt] = min(minVal[sccCnt], cost[pop]);
        } while (pop != u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> p;
    for (int i = 1; i <= n; i++) {
        cost[i] = INF;
    }
    for (int i = 1, u, c; i <= p; i++) {
        cin >> u >> c;
        cost[u] = c;
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];
        addEdge(a[i], b[i]);
    }
    for (int i = 1; i <= n; i++) {
        if (cost[i] != INF && dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (belong[i] == 0) {
            check = false;
            ans = i;
            break;
        }
    }
    if (!check) {
        cout << "NO" << "\n";
        cout << ans << "\n";
    } else {
        for (int i = 1; i <= m; i++) {
            int scc1 = belong[a[i]];
            int scc2 = belong[b[i]];
            if (scc1 != scc2) {
                indegree[scc2]++;
            }
        }
        for (int i = 1; i <= sccCnt; i++) {
            if (indegree[i] == 0) {
                ans += minVal[i];
            }
        }
        cout << "YES" << "\n";
        cout << ans << "\n";
    }
    return 0;
}
