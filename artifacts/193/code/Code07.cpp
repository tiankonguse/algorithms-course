#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 1001;
const int MAXM = 1001;
int t, n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

bool cutVertex[MAXN];
int vbccSiz[MAXN];
int vbccArr[MAXN << 1];
int vbccl[MAXN];
int vbccr[MAXN];
int idx;
int vbccCnt;

ll ans1, ans2;

void prepare() {
    cntg = cntd = top = idx = vbccCnt = 0;
    for (int i = 1; i < MAXN; i++) {
        head[i] = dfn[i] = low[i] = 0;
        cutVertex[i] = false;
    }
    n = 0;
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u, bool root) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    int son = 0;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            son++;
            tarjan(v, false);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                if (!root || son >= 2) {
                    cutVertex[u] = true;
                }
                vbccCnt++;
                vbccSiz[vbccCnt] = 1;
                vbccArr[++idx] = u;
                vbccl[vbccCnt] = idx;
                int pop;
                do {
                    pop = sta[top--];
                    vbccSiz[vbccCnt]++;
                    vbccArr[++idx] = pop;
                } while (pop != v);
                vbccr[vbccCnt] = idx;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

void compute() {
    if (vbccCnt == 1) {
        ans1 = 2;
        ans2 = 1ll * n * (n - 1) / 2;
    } else {
        ans1 = 0;
        ans2 = 1;
        for (int i = 1; i <= vbccCnt; i++) {
            int siz = vbccSiz[i], cut = 0;
            for (int j = vbccl[i]; j <= vbccr[i]; j++) {
                if (cutVertex[vbccArr[j]]) {
                    cut++;
                }
            }
            if (cut == 1) {
                ans1 += 1;
                ans2 = ans2 * (siz - 1);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    t = 0;
    cin >> m;
    while (m != 0) {
        prepare();
        for (int i = 1, u, v; i <= m; i++) {
            cin >> u >> v;
            n = max(n, u);
            n = max(n, v);
            addEdge(u, v);
            addEdge(v, u);
        }
        tarjan(1, true);
        compute();
        cout << "Case " << (++t) << ": " << ans1 << " " << ans2 << "\n";
        cin >> m;
    }
    return 0;
}
