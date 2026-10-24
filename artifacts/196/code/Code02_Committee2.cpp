#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
const int MAXM = 50001;
int n, m;

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
int sccCnt;

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
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
        } while (pop != u);
    }
}

int other(int x) {
    return (x & 1) == 0 ? (x - 1) : (x + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, other(v));
        addEdge(v, other(u));
    }
    for (int i = 1; i <= n << 1; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1, a, b; i <= n; i++) {
        a = i << 1;
        b = a - 1;
        if (belong[a] == belong[b]) {
            check = false;
        }
    }
    if (check) {
        for (int i = 1, a, b; i <= n; i++) {
            a = i << 1;
            b = a - 1;
            if (belong[a] < belong[b]) {
                cout << a << "\n";
            } else {
                cout << b << "\n";
            }
        }
    } else {
        cout << "NIE" << "\n";
    }
    return 0;
}
