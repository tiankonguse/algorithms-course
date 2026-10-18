#include <bits/stdc++.h>
using namespace std;

// 暴力：枚举所有 Prufer 序列（Cayley），对每棵生成树算 sumCost/sumDist 取最小
int n;
vector<int> X, Y, Z;

void decode(vector<int>& pr, vector<pair<int,int>>& edges) {
    int m = n;
    vector<int> deg(n + 1, 1);
    for (int v : pr) deg[v]++;
    // 用集合维护叶子
    set<int> leaves;
    for (int v = 1; v <= n; v++) if (deg[v] == 1) leaves.insert(v);
    for (int v : pr) {
        int leaf = *leaves.begin();
        leaves.erase(leaves.begin());
        edges.push_back({leaf, v});
        if (--deg[v] == 1) leaves.insert(v);
    }
    // 剩两个
    int a = *leaves.begin(), b = *next(leaves.begin());
    edges.push_back({a, b});
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        X.resize(n + 1); Y.resize(n + 1); Z.resize(n + 1);
        for (int i = 1; i <= n; i++) scanf("%d%d%d", &X[i], &Y[i], &Z[i]);
        double best = 1e100;
        int len = n - 2;
        int total = 1;
        for (int i = 0; i < len; i++) total *= n;
        for (int t = 0; t < total; t++) {
            int tmp = t;
            vector<int> pr(len);
            for (int i = 0; i < len; i++) { pr[i] = tmp % n + 1; tmp /= n; }
            vector<pair<int,int>> edges;
            decode(pr, edges);
            double sc = 0, sd = 0;
            for (auto& e : edges) {
                int a = e.first, b = e.second;
                sc += abs(Z[a] - Z[b]);
                sd += sqrt((double)(X[a] - X[b]) * (X[a] - X[b]) + (double)(Y[a] - Y[b]) * (Y[a] - Y[b]));
            }
            if (sd > 0) best = min(best, sc / sd);
        }
        printf("%.3f\n", best);
    }
    return 0;
}
