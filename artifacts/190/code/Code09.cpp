#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 100001;
const int MAXM = 300001;
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
int sccSiz[MAXN];
int sccCnt;

ll edgeArr[MAXM];
int cnte;

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
        sccSiz[sccCnt] = 0;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
            sccSiz[sccCnt]++;
        } while (pop != u);
    }
}

void condense() {
    cntg = 0;
    for (int i = 1; i <= sccCnt; i++) {
        head[i] = 0;
    }
    cnte = 0;
    for (int i = 1; i <= m; i++) {
        int scc1 = belong[a[i]];
        int scc2 = belong[b[i]];
        if (scc1 != scc2) {
            edgeArr[++cnte] = ((1LL * scc1) << 32) | scc2;
        }
    }
    sort(edgeArr + 1, edgeArr + cnte + 1);
    ll pre = 0, cur;
    for (int i = 1; i <= cnte; i++) {
        cur = edgeArr[i];
        if (cur != pre) {
            int scc1 = (int)(cur >> 32);
            int scc2 = (int)(cur & 0xffffffffLL);
            indegree[scc2]++;
            addEdge(scc1, scc2);
            pre = cur;
        }
    }
}

bool isolated(int i) {
    if (indegree[i] > 0 || sccSiz[i] > 1) {
        return false;
    }
    if (head[i] == 0) {
        return true;
    }
    for (int e = head[i]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (indegree[v] == 1) {
            return false;
        }
    }
    return true;
}

double compute() {
    int inZero = 0;
    for (int i = 1; i <= sccCnt; i++) {
        if (indegree[i] == 0) {
            inZero++;
        }
    }
    for (int i = 1; i <= sccCnt; i++) {
        if (isolated(i)) {
            inZero--;
            break;
        }
    }
    return 1.0 - (double)inZero / (double)n;
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &a[i], &b[i]);
        addEdge(a[i], b[i]);
    }
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    condense();
    double ans = compute();
    printf("%.6lf", ans);
    return 0;
}
