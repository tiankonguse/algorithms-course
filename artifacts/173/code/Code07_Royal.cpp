#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1001;
int n, b;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int capital[MAXN];
int belong[MAXN];
int cntb;

int sta[MAXN];
int siz;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs(int u, int f) {
    int x = siz;
    for (int e = head[u], v; e; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            dfs(v, u);
            if (siz - x >= b) {
                capital[++cntb] = u;
                while (siz != x) {
                    belong[sta[siz--]] = cntb;
                }
            }
        }
    }
    sta[++siz] = u;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> b;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs(1, 0);
    if (cntb == 0) {
        capital[++cntb] = 1;
    }
    while (siz > 0) {
        belong[sta[siz--]] = cntb;
    }
    cout << cntb << '\n';
    for (int i = 1; i <= n; i++) {
        cout << belong[i] << ' ';
    }
    cout << '\n';
    for (int i = 1; i <= cntb; i++) {
        cout << capital[i] << ' ';
    }
    cout << '\n';
    return 0;
}
