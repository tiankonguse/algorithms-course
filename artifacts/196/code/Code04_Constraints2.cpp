#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
const int MAXM = 2000001;
const int MAXK = 12;
const int MAXT = MAXN * MAXK * 2;
int t, n, m, k;

int id[MAXN][MAXK][2];
int cntt;

int head[MAXT];
int nxt[MAXM];
int to[MAXM];
int cntg;

int dfn[MAXT];
int low[MAXT];
int cntd;

int sta[MAXT];
int top;

int belong[MAXT];
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

void prepare() {
    cntg = cntd = sccCnt = 0;
    for (int i = 1; i <= cntt; i++) {
        head[i] = dfn[i] = low[i] = belong[i] = 0;
    }
    cntt = 0;
    for (int i = 1; i <= n; i++) {
        for (int v = 1; v <= k + 1; v++) {
            id[i][v][0] = ++cntt;
            id[i][v][1] = ++cntt;
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int v = 1; v <= k; v++) {
            addEdge(id[i][v][0], id[i][v + 1][0]);
            addEdge(id[i][v + 1][1], id[i][v][1]);
        }
    }
    for (int i = 1; i < n; i++) {
        for (int v = 1; v <= k; v++) {
            addEdge(id[i + 1][v][0], id[i][v][0]);
            addEdge(id[i][v][1], id[i + 1][v][1]);
        }
    }
    for (int i = 1; i <= n; i++) {
        addEdge(id[i][1][0], id[i][1][1]);
        addEdge(id[i][k + 1][1], id[i][k + 1][0]);
    }
}

void notEqual(int i, int x) {
    addEdge(id[i][x][1], id[i][x + 1][1]);
    addEdge(id[i][x + 1][0], id[i][x][0]);
}

void lessEqual(int i, int j, int x) {
    for (int w = 1; w <= k; w++) {
        if (w >= x) {
            notEqual(i, w);
        } else {
            if (x - w + 1 <= k) {
                addEdge(id[i][w][1], id[j][x - w + 1][0]);
                addEdge(id[j][x - w + 1][1], id[i][w][0]);
            }
        }
    }
}

void moreEqual(int i, int j, int x) {
    for (int w = 1; w <= k; w++) {
        if (w <= x - k - 1) {
            notEqual(i, w);
        } else {
            if (x - w > 1) {
                addEdge(id[i][w + 1][0], id[j][x - w][1]);
                addEdge(id[j][x - w][0], id[i][w + 1][1]);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> t;
    for (int c = 1; c <= t; c++) {
        cin >> n >> m >> k;
        prepare();
        for (int i = 1, op, x, y, v; i <= m; i++) {
            cin >> op;
            if (op == 1) {
                cin >> x >> v;
                notEqual(x, v);
            } else if (op == 2) {
                cin >> x >> y >> v;
                lessEqual(x, y, v);
                swap(x, y);
                lessEqual(x, y, v);
            } else {
                cin >> x >> y >> v;
                moreEqual(x, y, v);
                swap(x, y);
                moreEqual(x, y, v);
            }
        }
        for (int i = 1; i <= cntt; i++) {
            if (dfn[i] == 0) {
                tarjan(i);
            }
        }
        bool check = true;
        for (int i = 1; i <= n; i++) {
            for (int v = 1; v <= k + 1; v++) {
                if (belong[id[i][v][0]] == belong[id[i][v][1]]) {
                    check = false;
                    break;
                }
            }
            if (!check) {
                break;
            }
        }
        if (check) {
            for (int i = 1; i <= n; i++) {
                for (int v = k; v >= 1; v--) {
                    if (belong[id[i][v][1]] < belong[id[i][v][0]]) {
                        cout << v << " ";
                        break;
                    }
                }
            }
            cout << "\n";
        } else {
            cout << -1 << "\n";
        }
    }
    return 0;
}
