#include <bits/stdc++.h>
using namespace std;

// 暴力：DFS 枚举所有简单环，求最小平均权
int n, m;
vector<vector<pair<int,double>>> g;
vector<int> pathv;
vector<double> pathw;  // pathw[i] = pathv[i] -> pathv[i+1] 的边权
vector<int> pos;       // inpath 下节点在 pathv 的位置
vector<char> inpath;
double best = 1e100;

void dfs(int u) {
    pos[u] = pathv.size();
    pathv.push_back(u);
    inpath[u] = 1;
    for (auto& e : g[u]) {
        int v = e.first;
        double w = e.second;
        if (inpath[v]) {
            int pv = pos[v];
            double s = 0;
            for (int i = pv; i < (int)pathw.size(); i++) s += pathw[i];
            s += w;  // 闭合边 u -> v
            int c = (int)pathv.size() - pv;
            if (c > 0) best = min(best, s / c);
        } else {
            pathw.push_back(w);
            dfs(v);
            pathw.pop_back();
        }
    }
    inpath[u] = 0;
    pathv.pop_back();
}

int main() {
    scanf("%d%d", &n, &m);
    g.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        double w;
        scanf("%d%d%lf", &u, &v, &w);
        g[u].push_back({v, w});
    }
    pos.assign(n + 1, -1);
    inpath.assign(n + 1, 0);
    for (int s = 1; s <= n; s++) {
        if (!inpath[s]) dfs(s);
    }
    if (best > 9e99) best = 1e7;  // 无环，对齐课程 ans 上限
    printf("%.8f\n", best);
    return 0;
}
