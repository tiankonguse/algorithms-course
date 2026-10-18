#include <bits/stdc++.h>
using namespace std;
// 135 课 ShowDetails：同余方程组模板例子（mod 7）
int MOD = 7;
int MAXN = 101;
int mat[101][101];
int inv[7];

void invs() {
  inv[1] = 1;
  for (int i = 2; i < MOD; i++) {
    inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD);
  }
}
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
void gauss(int n) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (j < i && mat[j][j] != 0) continue;
      if (mat[j][i] != 0) { swap(mat[i], mat[j]); break; }
    }
    if (mat[i][i] != 0) {
      for (int j = 1; j <= n; j++) {
        if (i != j && mat[j][i] != 0) {
          int g = gcd(mat[j][i], mat[i][i]);
          int a = mat[i][i] / g;
          int b = mat[j][i] / g;
          if (j < i && mat[j][j] != 0) {
            for (int k = j; k < i; k++) {
              mat[j][k] = (mat[j][k] * a) % MOD;
            }
          }
          for (int k = i; k <= n + 1; k++) {
            mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
          }
        }
      }
    }
  }
  for (int i = 1; i <= n; i++) {
    if (mat[i][i] != 0) {
      bool flag = false;
      for (int j = i + 1; j <= n; j++) {
        if (mat[i][j] != 0) { flag = true; break; }
      }
      if (!flag) {
        mat[i][n + 1] = (mat[i][n + 1] * inv[mat[i][i]]) % MOD;
        mat[i][i] = 1;
      }
    }
  }
}
void prt(int n) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n + 1; j++) cout << mat[i][j] << " ";
    cout << endl;
  }
  cout << "========================" << endl;
}
int main() {
  invs();
  cout << "唯一解" << endl;
  mat[1][1]=4; mat[1][2]=2; mat[1][3]=4; mat[1][4]=3;
  mat[2][1]=2; mat[2][2]=5; mat[2][3]=2; mat[2][4]=2;
  mat[3][1]=6; mat[3][2]=3; mat[3][3]=4; mat[3][4]=5;
  gauss(3); prt(3);
  memset(mat, 0, sizeof(mat));
  cout << "矛盾" << endl;
  mat[1][1]=1; mat[1][2]=2; mat[1][3]=3; mat[1][4]=2;
  mat[2][1]=2; mat[2][2]=4; mat[2][3]=6; mat[2][4]=5;
  mat[3][1]=0; mat[3][2]=3; mat[3][3]=4; mat[3][4]=2;
  gauss(3); prt(3);
  memset(mat, 0, sizeof(mat));
  cout << "多解（主元依赖自由元）" << endl;
  mat[1][1]=1; mat[1][2]=2; mat[1][3]=3; mat[1][4]=2;
  mat[2][1]=2; mat[2][2]=4; mat[2][3]=6; mat[2][4]=4;
  mat[3][1]=0; mat[3][2]=3; mat[3][3]=4; mat[3][4]=2;
  gauss(3); prt(3);
  memset(mat, 0, sizeof(mat));
  cout << "多解（x1 可定，x2 依赖 x3）" << endl;
  mat[1][1]=1; mat[1][2]=1; mat[1][3]=1; mat[1][4]=3;
  mat[2][1]=2; mat[2][2]=1; mat[2][3]=1; mat[2][4]=5;
  mat[3][1]=0; mat[3][2]=3; mat[3][3]=3; mat[3][4]=3;
  gauss(3); prt(3);
  return 0;
}
