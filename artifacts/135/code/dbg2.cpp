#include <bits/stdc++.h>
using namespace std;
int MOD = 7;
int inv[7];
void invs() { inv[1] = 1; for (int i = 2; i < MOD; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD); }
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
int mat[302][302];
int n=1, m=1, s;
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
          if (j < i && mat[j][j] != 0) {
            for (int k = j; k < i; k++) mat[j][k] = (mat[j][k] * a) % MOD;
          }
          for (int k = i; k <= nn + 1; k++) mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
        }
      }
    }
  }
  for (int i = 1; i <= nn; i++) {
    if (mat[i][i] != 0) {
      bool flag = false;
      for (int j = i + 1; j <= nn; j++) if (mat[i][j] != 0) { flag = true; break; }
      if (!flag) {
        mat[i][nn + 1] = (mat[i][nn + 1] * inv[mat[i][i]]) % MOD;
        mat[i][i] = 1;
      }
    }
  }
}
int main() {
  invs();
  // n=1, 记录: k=1, st=0(MON), et=6(SUN), tool=1 → daydiff=(6-0+1)%7=0
  s = max(n, m);
  for (int i = 1; i <= s; i++) for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
  mat[1][1] = (mat[1][1] + 1) % 7;
  mat[1][s + 1] = ((6 - 0 + 1) % 7 + 7) % 7;
  cout << "消元前: " << mat[1][1] << " | " << mat[1][s+1] << endl;
  gauss(s);
  cout << "消元后: " << mat[1][1] << " | " << mat[1][s+1] << endl;
  return 0;
}
