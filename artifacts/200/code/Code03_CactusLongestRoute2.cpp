#include <bits/stdc++.h>

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
int sta[MAXN];
int top;

int cycle[MAXN];
int f[MAXN];
int g[MAXN];

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

int dpOnCycle(int u, int v) {
    int siz = 0;
    int pop;
    do {
        pop = sta[top--];
        cycle[++siz] = pop;
    } while (pop != v);
    int best = 0;
    int sum = 1;
    for (int i = 1; i <= siz; i++) {
        best = max(best, sum + g[cycle[i]]);
        sum += f[cycle[i]] + 1;
    }
    f[u] += sum;
    int otherSide = sum;
    for (int i = 1; i <= siz; i++) {
        otherSide -= f[cycle[i]] + 1;
        best = max(best, otherSide + g[cycle[i]]);
    }
    return best - sum;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    int delta = 0;
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
                top--;
                delta = max(delta, g[v] + 1);
            } else {
                delta = max(delta, dpOnCycle(u, v));
            }
        } else if (dfn[v] < dfn[u]) {
            low[u] = min(low[u], dfn[v]);
        }
    }
    g[u] = f[u] + delta;
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
    tarjan(1, 0);
    cout << g[1] << "\n";
    return 0;
}
