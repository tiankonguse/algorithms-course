#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXM = 500001;
int n, a, b;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

bool isKey[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u] && u != a && u != b && dfn[b] >= dfn[v]) {
                isKey[u] = true;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    int u, v;
    cin >> u >> v;
    while (u != 0 || v != 0) {
        addEdge(u, v);
        addEdge(v, u);
        cin >> u >> v;
    }
    cin >> a >> b;
    tarjan(a);
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (isKey[i]) {
            ans = i;
            break;
        }
    }
    if (ans == 0) {
        cout << "No solution" << "\n";
    } else {
        cout << ans << "\n";
    }
    return 0;
}
