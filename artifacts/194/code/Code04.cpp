#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 100001;
const int MAXM = 500001;
int n, m, cntn;

int head1[MAXN];
int next1[MAXM << 1];
int to1[MAXM << 1];
int cnt1;

int head2[MAXN << 1];
int next2[MAXM << 2];
int to2[MAXM << 2];
int cnt2;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int siz[MAXN << 1];
ll ans[MAXN];

void addEdge1(int u, int v) {
    next1[++cnt1] = head1[u];
    to1[cnt1] = v;
    head1[u] = cnt1;
}

void addEdge2(int u, int v) {
    next2[++cnt2] = head2[u];
    to2[cnt2] = v;
    head2[u] = cnt2;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head1[u]; e > 0; e = next1[e]) {
        int v = to1[e];
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                cntn++;
                addEdge2(cntn, u);
                addEdge2(u, cntn);
                int pop;
                do {
                    pop = sta[top--];
                    addEdge2(cntn, pop);
                    addEdge2(pop, cntn);
                } while (pop != v);
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

void dpOnTree(int u, int fa) {
    for (int e = head2[u]; e > 0; e = next2[e]) {
        int v = to2[e];
        if (v != fa) {
            dpOnTree(v, u);
            if (u <= n) {
                ans[u] += 1LL * siz[v] * (n - siz[v] - 1);
            }
            siz[u] += siz[v];
        }
    }
    siz[u] += u <= n ? 1 : 0;
    if (u <= n) {
        ans[u] += 1LL * (n - siz[u]) * (siz[u] - 1);
        ans[u] += 2LL * (n - 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    cntn = n;
    for (int i = 1, u, v; i <= m; i++) {
        cin >> u >> v;
        addEdge1(u, v);
        addEdge1(v, u);
    }
    tarjan(1);
    dpOnTree(1, 0);
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << "\n";
    }
    return 0;
}
