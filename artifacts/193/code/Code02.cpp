#include <iostream>
#include <algorithm>

using namespace std;

const int MAXN = 10001;
const int MAXM = 20001;
int n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int block, ans;

void prepare() {
    cntg = cntd = block = ans = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = dfn[i] = low[i] = 0;
    }
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, bool root) {
    dfn[u] = low[u] = ++cntd;
    int son = 0, cut = 0;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            son++;
            tarjan(v, false);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u] && !root) {
                cut++;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (root) {
        ans = max(ans, son);
    } else {
        ans = max(ans, cut + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    while (n != 0 || m != 0) {
        prepare();
        for (int i = 1, u, v; i <= m; i++) {
            cin >> u >> v;
            u++;
            v++;
            addEdge(u, v);
            addEdge(v, u);
        }
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                block++;
                tarjan(i, true);
            }
        }
        cout << (block + (ans - 1)) << "\n";
        cin >> n >> m;
    }
    return 0;
}
