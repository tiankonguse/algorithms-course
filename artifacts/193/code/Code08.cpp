#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1001;
const int MAXM = 1000001;
int n, m;
bool hate[MAXN][MAXN];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int vbccArr[MAXN << 1];
int vbccl[MAXN];
int vbccr[MAXN];
int idx;
int vbccCnt;

bool curVbcc[MAXN];
int color[MAXN];
bool ok[MAXN];

void prepare() {
    cntg = cntd = top = idx = vbccCnt = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = dfn[i] = low[i] = 0;
        ok[i] = false;
        for (int j = 1; j <= n; j++) {
            hate[i][j] = false;
        }
    }
}

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
            if (low[v] >= dfn[u]) {
                vbccCnt++;
                vbccArr[++idx] = u;
                vbccl[vbccCnt] = idx;
                int pop;
                do {
                    pop = sta[top--];
                    vbccArr[++idx] = pop;
                } while (pop != v);
                vbccr[vbccCnt] = idx;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

bool oddLoop(int u, int c) {
    color[u] = c;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (curVbcc[v]) {
            if (color[v] == c) {
                return true;
            }
            if (color[v] == 0 && oddLoop(v, c == 1 ? 2 : 1)) {
                return true;
            }
        }
    }
    return false;
}

int compute() {
    for (int i = 1; i <= vbccCnt; i++) {
        for (int j = vbccl[i]; j <= vbccr[i]; j++) {
            curVbcc[vbccArr[j]] = true;
            color[vbccArr[j]] = 0;
        }
        bool check = oddLoop(vbccArr[vbccl[i]], 1);
        for (int j = vbccl[i]; j <= vbccr[i]; j++) {
            curVbcc[vbccArr[j]] = false;
            ok[vbccArr[j]] = ok[vbccArr[j]] | check;
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (!ok[i]) {
            ans++;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    while (n != 0 || m != 0) {
        prepare();
        for (int i = 1, u, v; i <= m; i++) {
            cin >> u >> v;
            hate[u][v] = true;
            hate[v][u] = true;
        }
        for (int u = 1; u <= n; u++) {
            for (int v = u + 1; v <= n; v++) {
                if (!hate[u][v]) {
                    addEdge(u, v);
                    addEdge(v, u);
                }
            }
        }
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                tarjan(i);
            }
        }
        int ans = compute();
        cout << ans << "\n";
        cin >> n >> m;
    }
    return 0;
}
