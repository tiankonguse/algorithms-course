#include <bits/stdc++.h>

using namespace std;

const int MAXN = 300001;
const int MAXM = 300001;
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
int ebccCnt;

int dist[MAXN];
int diameter;

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

void dpOnTree(int u, int fa) {
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dpOnTree(v, u);
            diameter = max(diameter, dist[u] + dist[v] + 1);
            dist[u] = max(dist[u], dist[v] + 1);
        }
    }
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
    dpOnTree(1, 0);
    cout << diameter << "\n";
    return 0;
}
