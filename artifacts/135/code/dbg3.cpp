#include <bits/stdc++.h>
using namespace std;
int MOD = 7;
int inv[7];
void invs() { inv[1] = 1; for (int i = 2; i < MOD; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD); }
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
int main() {
  invs();
  // 直接用 paipai2 里的 Course 与 Ref，构造 n=1 m=2 rec=[[1,3],[1,3]]
  // 但为最小化，直接模拟 mat：
  int mat[5][5];
  memset(mat, 0, sizeof(mat));
  int n=1, m=2, s=max(n,m);
  for (int i = 1; i <= s; i++) for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
  mat[1][1] = 1; mat[1][s+1] = 3;
  mat[2][1] = 1; mat[2][s+1] = 3;
  cout << "初始: r1=" << mat[1][1] << "," << mat[1][s+1] << " r2=" << mat[2][1] << "," << mat[2][s+1] << endl;
  for (int i = 1; i <= s; i++) {
    for (int j = 1; j <= s; j++) {
      if (j < i && mat[j][j] != 0) continue;
      if (mat[j][i] != 0) { swap(mat[i], mat[j]); break; }
    }
    if (mat[i][i] != 0) {
      for (int j = 1; j <= s; j++) {
        if (i != j && mat[j][i] != 0) {
          int g = gcd(mat[j][i], mat[i][i]);
          int a = mat[i][i] / g;
          int b = mat[j][i] / g;
          if (j < i && mat[j][j] != 0) {
            for (int k = j; k < i; k++) mat[j][k] = (mat[j][k] * a) % MOD;
          }
          for (int k = i; k <= s + 1; k++) mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
        }
      }
    }
  }
  for (int i = 1; i <= s; i++) {
    if (mat[i][i] != 0) {
      bool flag = false;
      for (int j = i + 1; j <= s; j++) if (mat[i][j] != 0) { flag = true; break; }
      if (!flag) {
        mat[i][s + 1] = (mat[i][s + 1] * inv[mat[i][i]]) % MOD;
        mat[i][i] = 1;
      }
    }
  }
  cout << "消元后: r1=" << mat[1][1] << "," << mat[1][s+1] << " r2=" << mat[2][1] << "," << mat[2][s+1] << endl;
  return 0;
}
