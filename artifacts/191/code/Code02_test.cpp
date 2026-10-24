#include <bits/stdc++.h>
using namespace std;
// 包装：0-based 输入，n m，m 条边，输出每条割边的两个端点
#include "Code02.cpp"
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> conn(m);
    for (int i = 0; i < m; i++) { int a, b; cin >> a >> b; conn[i] = {a, b}; }
    auto ans = Solution().criticalConnections(n, conn);
    for (auto& p : ans) cout << p[0] << " " << p[1] << "\n";
    return 0;
}
