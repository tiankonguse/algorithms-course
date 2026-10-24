#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 100001;
const int MAXM = 1000001;
int n, m, x;
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
int sccSiz[MAXN];
int sccCnt;

ll edgeArr[MAXM];
int cnte;

int indegree[MAXN];
int dpSum[MAXN];
int dpCnt[MAXN];

int ans1, ans2;

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
        sccSiz[sccCnt] = 0;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
            sccSiz[sccCnt]++;
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
        if (scc1 != scc2) {
            edgeArr[++cnte] = ((1LL * scc1) << 32) | scc2;
        }
    }
    sort(edgeArr + 1, edgeArr + cnte + 1);
    ll pre = 0, cur;
    for (int i = 1; i <= cnte; i++) {
        cur = edgeArr[i];
        if (cur != pre) {
            int scc1 = (int)(cur >> 32);
            int scc2 = (int)(cur & 0xffffffffLL);
            indegree[scc2]++;
            addEdge(scc1, scc2);
            pre = cur;
        }
    }
}

void dpOnDAG() {
    for (int u = sccCnt; u > 0; u--) {
        if (indegree[u] == 0) {
            dpSum[u] = sccSiz[u];
            dpCnt[u] = 1;
        }
        for (int e = head[u]; e > 0; e = nxt[e]) {
            int v = to[e];
            if (dpSum[v] < dpSum[u] + sccSiz[v]) {
                dpSum[v] = dpSum[u] + sccSiz[v];
                dpCnt[v] = dpCnt[u];
            } else if (dpSum[v] == dpSum[u] + sccSiz[v]) {
                dpCnt[v] = (dpCnt[v] + dpCnt[u]) % x;
            }
        }
    }
    ans1 = ans2 = 0;
    for (int i = 1; i <= sccCnt; i++) {
        if (dpSum[i] > ans1) {
            ans1 = dpSum[i];
            ans2 = dpCnt[i];
        } else if (dpSum[i] == ans1) {
            ans2 = (ans2 + dpCnt[i]) % x;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> x;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i];
        addEdge(a[i], b[i]);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    condense();
    dpOnDAG();
    cout << ans1 << "\n";
    cout << ans2 << "\n";
    return 0;
}
