#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
const int MAXM = 100001;
int n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

bool cutVertex[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, bool root) {
    dfn[u] = low[u] = ++cntd;
    int son = 0;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            son++;
            tarjan(v, false);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                if (!root || son >= 2) {
                    cutVertex[u] = true;
                }
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i, true);
        }
    }
    int cutCnt = 0;
    for (int i = 1; i <= n; i++) {
        if (cutVertex[i]) {
            cutCnt++;
        }
    }
    cout << cutCnt << "\n";
    for (int i = 1; i <= n; i++) {
        if (cutVertex[i]) {
            cout << i << " ";
        }
    }
    cout << "\n";
    return 0;
}
