#include <bits/stdc++.h>
using namespace std;
int MOD = 3;
int inv[3];
void invs() { inv[1] = 1; for (int i = 2; i < MOD; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD); }
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
int mat[1001][1001];
int dir[] = {0, -1, 0, 1, 0};
int n=2, m=2, s=4;
void prepare() {
  for (int i = 1; i <= s; i++) for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
  int cur, row, col;
  for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
    cur = i * m + j + 1;
    mat[cur][cur] = 2;
    for (int d = 0; d <= 3; d++) {
      row = i + dir[d]; col = j + dir[d + 1];
      if (row >= 0 && row < n && col >= 0 && col < m) mat[cur][row * m + col + 1] = 1;
    }
  }
}
void gauss(int nn) {
  for (int i = 1; i <= nn; i++) {
    for (int j = 1; j <= nn; j++) {
      if (j < i && mat[j][j] != 0) continue;
      if (mat[j][i] != 0) { swap(mat[i], mat[j]); break; }
    }
    if (mat[i][i] != 0) {
      for (int j = 1; j <= nn; j++) {
        if (i != j && mat[j][i] != 0) {
          int g = gcd(mat[j][i], mat[i][i]);
          int a = mat[i][i] / g;
          int b = mat[j][i] / g;
          if (j < i && mat[j][j] != 0) mat[j][j] = (mat[j][j] * a) % MOD;
          for (int k = i; k <= nn + 1; k++) mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
        }
      }
    }
  }
  for (int i = 1; i <= nn; i++) if (mat[i][i] != 0) mat[i][nn + 1] = (mat[i][nn + 1] * inv[mat[i][i]]) % MOD;
}
int main() {
  invs();
  prepare();
  int init[5] = {0, 0, 1, 1, 0}; // init[1..4] = 0,1,1,0
  for (int i = 1; i <= s; i++) mat[i][s + 1] = (3 - init[i]) % MOD;
  cout << "=== 消元前 A ===" << endl;
  for (int i = 1; i <= s; i++) { for (int j = 1; j <= s + 1; j++) cout << mat[i][j] << " "; cout << endl; }
  gauss(s);
  cout << "=== 消元后 ===" << endl;
  for (int i = 1; i <= s; i++) { for (int j = 1; j <= s + 1; j++) cout << mat[i][j] << " "; cout << endl; }
  cout << "x = ";
  int x[5]; for (int i = 1; i <= s; i++) x[i] = mat[i][s + 1];
  for (int i = 1; i <= s; i++) cout << x[i] << " "; cout << endl;
  // 验证 A*x
  for (int i = 1; i <= s; i++) {
    int sum = 0;
    for (int j = 1; j <= s; j++) sum = (sum + mat[i][j] * x[j]) % MOD; // 注意此时 mat 已被消元，不能直接用
  }
  // 重新构造原始 A 验证
  prepare();
  for (int i = 1; i <= s; i++) {
    int sum = 0;
    for (int j = 1; j <= s; j++) sum = (sum + mat[i][j] * x[j]) % MOD;
    cout << "格" << i << " 加成=" << sum << " 目标=" << mat[i][s+1] << (sum==mat[i][s+1]?" OK":" BAD") << endl;
  }
  return 0;
}
