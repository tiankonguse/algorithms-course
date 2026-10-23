#include <bits/stdc++.h>

using namespace std;

const int MAXN = 5001;
const int MAXM = 100001;
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
int sccSiz[MAXN];
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
            sccSiz[sccCnt]++;
        } while (pop != u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, a, b, t; i <= m; i++) {
        cin >> a >> b >> t;
        if (t == 1) {
            addEdge(a, b);
        } else {
            addEdge(a, b);
            addEdge(b, a);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    int largest = 0;
    for (int i = 1; i <= sccCnt; i++) {
        largest = max(largest, sccSiz[i]);
    }
    cout << largest << "\n";
    for (int i = 1; i <= n; i++) {
        if (sccSiz[belong[i]] == largest) {
            int scc = belong[i];
            for (int j = i; j <= n; j++) {
                if (belong[j] == scc) {
                    cout << j << " ";
                }
            }
            break;
        }
    }
    cout << "\n";
    return 0;
}
