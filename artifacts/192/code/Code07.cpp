#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 500001;
const int MAXM = 1000001;
const int MOD = 1000000007;
int n, m;
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
int ebccSiz[MAXN];
int ebccCnt;

ll power2[MAXM];
ll dp[MAXN];
int bridge[MAXN];

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
        ebccSiz[ebccCnt] = 0;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = ebccCnt;
            ebccSiz[ebccCnt]++;
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

void dpOnTree(int u, int fa) {
    dp[u] = power2[ebccSiz[u]] - 1;
    bridge[u] = 0;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dpOnTree(v, u);
            dp[u] = (dp[u] * power2[bridge[v]] * 2 % MOD
                    + power2[bridge[u]] * dp[v] % MOD
                    + dp[u] * dp[v] % MOD) % MOD;
            bridge[u] += bridge[v] + 1;
        }
    }
}

ll compute() {
    power2[0] = 1;
    for (int i = 1; i <= m; i++) {
        power2[i] = power2[i - 1] * 2 % MOD;
    }
    dpOnTree(1, 0);
    int total = bridge[1];
    ll ans = dp[1];
    for (int i = 2; i <= ebccCnt; i++) {
        ans = (ans + dp[i] * power2[total - bridge[i] - 1] % MOD) % MOD;
    }
    ans = ans * power2[m - total] % MOD;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cntg = 1;
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];
        addEdge(a[i], b[i]);
        addEdge(b[i], a[i]);
    }
    tarjan(1, 0);
    condense();
    ll ans = compute();
    cout << ans << "\n";
    return 0;
}
