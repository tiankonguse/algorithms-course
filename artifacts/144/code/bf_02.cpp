#include <bits/stdc++.h>
using namespace std;
const int MOD = 10007;
// 暴力：直接展开 (a*x + b*y)^k，取 x^n * y^m 系数 % MOD
int main() {
  int a, b, k, n, m; cin >> a >> b >> k >> n >> m;
  static long long coef[40][40];
  memset(coef, 0, sizeof(coef));
  coef[0][0] = 1 % MOD;
  for (int step = 0; step < k; step++) {
    static long long nx[40][40];
    memset(nx, 0, sizeof(nx));
    for (int i = 0; i <= step; i++)
      for (int j = 0; j <= step; j++) {
        if (!coef[i][j]) continue;
        nx[i+1][j] = (nx[i+1][j] + coef[i][j] * (a % MOD)) % MOD; // 乘 a*x
        nx[i][j+1] = (nx[i][j+1] + coef[i][j] * (b % MOD)) % MOD; // 乘 b*y
      }
    memcpy(coef, nx, sizeof(coef));
  }
  cout << coef[n][m] % MOD << "\n";
}
