#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXM = 500001;
int n, m;
int age[MAXN];
int sumAge;

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

void prepare() {
    sumAge = cntg = cntd = sccCnt = 0;
    top = 0;
    for (int i = 1; i <= n << 1; i++) {
        head[i] = dfn[i] = low[i] = belong[i] = 0;
    }
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

bool older(int x) {
    return age[x] * n >= sumAge;
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
    cin >> n >> m;
    while (n != 0 && m != 0) {
        prepare();
        for (int i = 1; i <= n; i++) {
            cin >> age[i];
            sumAge += age[i];
        }
        for (int i = 1, x, y; i <= m; i++) {
            cin >> x >> y;
            addEdge(x + n, y);
            addEdge(y + n, x);
            if (older(x) == older(y)) {
                addEdge(x, y + n);
                addEdge(y, x + n);
            }
        }
        for (int i = 1; i <= n << 1; i++) {
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
        if (check) {
            for (int i = 1; i <= n; i++) {
                if (belong[i] < belong[i + n]) {
                    if (older(i)) {
                        cout << "A" << "\n";
                    } else {
                        cout << "B" << "\n";
                    }
                } else {
                    cout << "C" << "\n";
                }
            }
        } else {
            cout << "No solution." << "\n";
        }
        cin >> n >> m;
    }
    return 0;
}
