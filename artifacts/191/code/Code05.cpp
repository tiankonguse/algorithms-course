#include <bits/stdc++.h>

using namespace std;

const int MAXN = 500001;
const int MAXM = 2000001;
int n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int ebccSiz[MAXN];
int ebccArr[MAXN];
int ebccl[MAXN];
int ebccr[MAXN];
int idx;
int ebccCnt;

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
        ebccl[ebccCnt] = idx + 1;
        int pop;
        do {
            pop = sta[top--];
            ebccSiz[ebccCnt]++;
            ebccArr[++idx] = pop;
        } while (pop != u);
        ebccr[ebccCnt] = idx;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cntg = 1;
    cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i, 0);
        }
    }
    cout << ebccCnt << "\n";
    for (int i = 1; i <= ebccCnt; i++) {
        cout << ebccSiz[i] << "\n";
        for (int j = ebccl[i]; j <= ebccr[i]; j++) {
            cout << ebccArr[j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
