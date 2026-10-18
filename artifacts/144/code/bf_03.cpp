#include <bits/stdc++.h>
using namespace std;
// 暴力：独立建 %k 组合表（复用同款递推但只到 40），每组查询逐 (i,j) 计数
int main() {
  int t, k; cin >> t >> k;
  static int cc[41][41];
  for (int i = 0; i <= 40; i++) {
    cc[i][0] = 1 % k;
    for (int j = 1; j <= i; j++) cc[i][j] = (cc[i-1][j] + cc[i-1][j-1]) % k;
  }
  while (t--) {
    int n, m; cin >> n >> m;
    int ans = 0;
    for (int i = 0; i <= n; i++)
      for (int j = 0; j <= min(i, m); j++)
        if (cc[i][j] == 0) ans++;
    cout << ans << "\n";
  }
}
