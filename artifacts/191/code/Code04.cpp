#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 300001;
int n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

bool check;
int ans1[MAXM];
int ans2[MAXM];
int cnta;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0 || dfn[v] < dfn[u]) {
            cnta++;
            ans1[cnta] = u;
            ans2[cnta] = v;
        }
        if (dfn[v] == 0) {
            tarjan(v, e);
            low[u] = min(low[u], low[v]);
            if (low[v] > dfn[u]) {
                check = false;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
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
    check = true;
    tarjan(1, 0);
    if (!check) {
        cout << 0 << "\n";
    } else {
        for (int i = 1; i <= m; i++) {
            cout << ans1[i] << " " << ans2[i] << "\n";
        }
    }
    return 0;
}
