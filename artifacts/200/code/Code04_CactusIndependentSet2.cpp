#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 200001;
const int INF = 1000000000;
int n, m;
int arr[MAXN];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;
int sta[MAXN];
int top;

int cycle[MAXN];
int dp[MAXN][2];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dpOnCycle(int u, int v) {
    int siz = 0;
    int pop;
    do {
        pop = sta[top--];
        cycle[++siz] = pop;
    } while (pop != v);
    int pre0 = dp[cycle[1]][0], pre1 = dp[cycle[1]][1];
    int cur0, cur1;
    for (int i = 2; i <= siz; i++) {
        int x = cycle[i];
        cur0 = max(pre0, pre1) + dp[x][0];
        cur1 = pre0 + dp[x][1];
        pre0 = cur0;
        pre1 = cur1;
    }
    dp[u][0] += max(pre0, pre1);
    pre0 = dp[cycle[1]][0];
    pre1 = -INF;
    for (int i = 2; i <= siz; i++) {
        int x = cycle[i];
        cur0 = max(pre0, pre1) + dp[x][0];
        cur1 = pre0 + dp[x][1];
        pre0 = cur0;
        pre1 = cur1;
    }
    dp[u][1] += pre0;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    dp[u][0] = 0;
    dp[u][1] = arr[u];
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v, e);
            if (low[v] < dfn[u]) {
                low[u] = min(low[u], low[v]);
            } else if (low[v] > dfn[u]) {
                dp[u][0] += max(dp[v][0], dp[v][1]);
                dp[u][1] += dp[v][0];
                top--;
            } else {
                dpOnCycle(u, v);
            }
        } else if (dfn[v] < dfn[u]) {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    cntg = 1;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }

    // 洛谷P4410，使用如下的点权设置
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 洛谷P10779，使用如下的点权设置
    // for (int i = 1; i <= n; i++) {
    //     arr[i] = 1;
    // }

    tarjan(1, 0);
    cout << max(dp[1][0], dp[1][1]) << "\n";
    return 0;
}
