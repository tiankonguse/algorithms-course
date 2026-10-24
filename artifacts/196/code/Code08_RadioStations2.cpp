#include <bits/stdc++.h>

using namespace std;

const int MAXN = 400001;
const int MAXT = 2000001;
const int MAXM = 4000001;
int n, p, f, m, cntt;

int l[MAXN];
int r[MAXN];

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

int pre[MAXN];
int suf[MAXN];

int ansx;
int pick[MAXN];
int siz;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void link() {
    pre[1] = ++cntt;
    for (int i = 2; i <= f; i++) {
        pre[i] = ++cntt;
        addEdge(pre[i - 1], pre[i]);
    }
    suf[f] = ++cntt;
    for (int i = f - 1; i >= 1; i--) {
        suf[i] = ++cntt;
        addEdge(suf[i + 1], suf[i]);
    }
    for (int i = 1; i <= p; i++) {
        addEdge(i, pre[r[i]]);
        addEdge(i, suf[l[i]]);
        if (l[i] - 1 >= 1) {
            addEdge(pre[l[i] - 1], i + p);
        }
        if (r[i] + 1 <= f) {
            addEdge(suf[r[i] + 1], i + p);
        }
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

bool compute() {
    bool check = true;
    for (int i = 1; i <= p; i++) {
        if (belong[i] == belong[i + p]) {
            check = false;
            break;
        } else if (belong[i] < belong[i + p]) {
            ansx = max(ansx, l[i]);
            pick[++siz] = i;
        }
    }
    return check;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> p >> f >> m;
    cntt = p << 1;
    for (int i = 1, u, v; i <= n; i++) {
        cin >> u >> v;
        addEdge(u + p, v);
        addEdge(v + p, u);
    }
    for (int i = 1; i <= p; i++) {
        cin >> l[i] >> r[i];
    }
    link();
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge(u, v + p);
        addEdge(v, u + p);
    }
    for (int i = 1; i <= cntt; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = compute();
    if (check) {
        cout << siz << " " << ansx << "\n";
        for (int i = 1; i <= siz; i++) {
            cout << pick[i] << " ";
        }
    } else {
        cout << -1 << "\n";
    }
    return 0;
}
