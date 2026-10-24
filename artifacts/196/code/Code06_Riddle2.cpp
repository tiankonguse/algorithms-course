#include <bits/stdc++.h>

using namespace std;

const int MAXN = 5000001;
const int MAXM = 10000001;
int n, m, k, w, cntt;
int arr[MAXN];

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

void link() {
    cntt++;
    addEdge(cntt, arr[1] + n);
    for (int i = 2; i <= w; i++) {
        cntt++;
        addEdge(cntt, arr[i] + n);
        addEdge(arr[i], cntt - 1);
        addEdge(cntt, cntt - 1);
    }
    cntt++;
    addEdge(cntt, arr[w] + n);
    for (int i = w - 1; i >= 1; i--) {
        cntt++;
        addEdge(cntt, arr[i] + n);
        addEdge(arr[i], cntt - 1);
        addEdge(cntt, cntt - 1);
    }
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
    cin >> n >> m >> k;
    cntt = n << 1;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u + n, v);
        addEdge(v + n, u);
    }
    for (int i = 1; i <= k; i++) {
        cin >> w;
        for (int j = 1; j <= w; j++) {
            cin >> arr[j];
        }
        link();
    }
    for (int i = 1; i <= cntt; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1; i <= n; i++) {
        if (belong[i] == belong[i + n]) {
            check = false;
            break;
        }
    }
    cout << (check ? "TAK" : "NIE") << "\n";
    return 0;
}
