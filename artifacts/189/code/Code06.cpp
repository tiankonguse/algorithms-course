#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 500001;
int n, m;
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
int sccCnt;

int outdegree[MAXN];
int indegree[MAXN];

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    m = 0;
    for (int i = 1, x; i <= n; i++) {
        cin >> x;
        while (x != 0) {
            m++;
            a[m] = i;
            b[m] = x;
            addEdge(a[m], b[m]);
            cin >> x;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    if (sccCnt == 1) {
        cout << "1" << "\n";
        cout << "0" << "\n";
    } else {
        for (int i = 1; i <= m; i++) {
            int scc1 = belong[a[i]];
            int scc2 = belong[b[i]];
            if (scc1 != scc2) {
                outdegree[scc1]++;
                indegree[scc2]++;
            }
        }
        int outZero = 0;
        int inZero = 0;
        for (int i = 1; i <= sccCnt; i++) {
            if (outdegree[i] == 0) {
                outZero++;
            }
            if (indegree[i] == 0) {
                inZero++;
            }
        }
        cout << inZero << "\n";
        cout << max(outZero, inZero) << "\n";
    }
    return 0;
}
