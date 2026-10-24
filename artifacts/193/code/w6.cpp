#include <bits/stdc++.h>
using namespace std;
#include "Code06.cpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    while (cin >> n >> m) {
        vector<vector<int>> roads(m);
        for (int i = 0; i < m; i++) {
            int u, v; cin >> u >> v;
            roads[i] = {u, v};
        }
        vector<int> cost(n);
        for (int i = 0; i < n; i++) cin >> cost[i];
        Solution sol;
        cout << sol.minimumCost(cost, roads) << "\n";
    }
    return 0;
}
