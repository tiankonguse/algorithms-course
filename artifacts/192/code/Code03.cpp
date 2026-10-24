#include <bits/stdc++.h>

using namespace std;

const int MAXN = 500001;
const int MAXM = 1000001;
int n, m, maxv;
int a[MAXM];
int b[MAXM];
int c[MAXM];

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int weight[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int ebccCnt;

int dist[MAXN];
int diameter;
int edgeCnt;

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
    head[u] = cntg;
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v, e);
            low[u] = min(low[u], low[v]);
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        ebccCnt++;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = ebccCnt;
        } while (pop != u);
    }
}

void condense() {
    cntg = 0;
    for (int i = 1; i <= ebccCnt; i++) {
        head[i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int ebcc1 = belong[a[i]];
        int ebcc2 = belong[b[i]];
        int w = c[i];
        if (ebcc1 != ebcc2) {
            addEdge(ebcc1, ebcc2, w);
            addEdge(ebcc2, ebcc1, w);
        }
    }
}

void dpOnTree(int u, int fa, int limit) {
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (v != fa) {
            dpOnTree(v, u, limit);
            int w = weight[e] <= limit ? 1 : 0;
            edgeCnt += w;
            diameter = max(diameter, dist[u] + dist[v] + w);
            dist[u] = max(dist[u], dist[v] + w);
        }
    }
}

bool check(int limit) {
    for (int i = 1; i <= ebccCnt; i++) {
    	dist[i] = 0;
    }
    diameter = edgeCnt = 0;
    dpOnTree(1, 0, limit);
    return diameter < edgeCnt;
}

int compute() {
    int l = 1, r = maxv, mid, ans = -1;
    while (l <= r) {
        mid = (l + r) / 2;
        if (check(mid)) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cntg = 1;
    cin >> n >> m;
    maxv = 0;
    for (int i = 1; i <= m; i++) {
        cin >> a[i] >> b[i] >> c[i];
        addEdge(a[i], b[i], 0);
        addEdge(b[i], a[i], 0);
        maxv = max(maxv, c[i]);
    }
    tarjan(1, 0);
    condense();
    cout << compute() << "\n";
    return 0;
}
