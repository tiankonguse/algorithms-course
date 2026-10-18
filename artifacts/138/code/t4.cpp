#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3001;
const int MAXM = 10001;
const double MAXE = 1e7;
const double sml = 1e-9;

int n, m;
int head[MAXN], nxt[MAXM], to[MAXM];
double wgt[MAXM];
int cnt;
double value[MAXN];
bool path[MAXN];

void prepare() {
    cnt = 1;
    fill(head + 1, head + n + 1, 0);
}

void addEdge(int u, int v, double w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    wgt[cnt] = w;
    head[u] = cnt++;
}

bool dfs(int u, double x) {
    if (u == 0) {
        for (int i = 1; i <= n; i++) {
            if (dfs(i, x)) return true;
        }
    } else {
        path[u] = true;
        for (int e = head[u]; e != 0; e = nxt[e]) {
            int v = to[e];
            double w = wgt[e] - x;
            if (value[v] > value[u] + w) {
                value[v] = value[u] + w;
                if (path[v] || dfs(v, x)) return true;
            }
        }
        path[u] = false;
    }
    return false;
}

bool check(double x) {
    fill(value + 1, value + n + 1, 0);
    fill(path + 1, path + n + 1, false);
    return dfs(0, x);
}

int main() {
    scanf("%d%d", &n, &m);
    prepare();
    for (int i = 0; i < m; i++) {
        int u, v;
        double w;
        scanf("%d%d%lf", &u, &v, &w);
        addEdge(u, v, w);
    }
    double l = -MAXE, r = MAXE, ans = 0;
    while (l < r && r - l >= sml) {
        double x = (l + r) / 2;
        if (check(x)) {
            r = x - sml;
        } else {
            ans = x;
            l = x + sml;
        }
    }
    printf("%.8f\n", ans);
    return 0;
}
