#include <bits/stdc++.h>
using namespace std;
// 135 课题1 HDU5755 Gambler Bo：格子全变 0（mod 3 简化版，自由元不操作）
int MOD = 3;
int MAXS = 1001;
int mat[1001][1001];
int dir[] = {0, -1, 0, 1, 0};
int n, m, s;
int inv[3];

void invs() {
  inv[1] = 1;
  for (int i = 2; i < MOD; i++) {
    inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD);
  }
}
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
void prepare() {
  for (int i = 1; i <= s; i++)
    for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
  int cur, row, col;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cur = i * m + j + 1;
      mat[cur][cur] = 2;
      for (int d = 0; d <= 3; d++) {
        row = i + dir[d];
        col = j + dir[d + 1];
        if (row >= 0 && row < n && col >= 0 && col < m) {
          mat[cur][row * m + col + 1] = 1;
        }
      }
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
          if (j < i && mat[j][j] != 0) {
            mat[j][j] = (mat[j][j] * a) % MOD;
          }
          for (int k = i; k <= nn + 1; k++) {
            mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
          }
        }
      }
    }
  }
  for (int i = 1; i <= nn; i++) {
    if (mat[i][i] != 0) {
      mat[i][nn + 1] = (mat[i][nn + 1] * inv[mat[i][i]]) % MOD;
    }
  }
}
int main() {
  invs();
  int test;
  if (!(cin >> test)) return 0;
  while (test--) {
    cin >> n >> m;
    s = n * m;
    prepare();
    for (int i = 1; i <= s; i++) {
      int v; cin >> v;
      mat[i][s + 1] = (3 - v) % MOD;
    }
    gauss(s);
    int ans = 0;
    for (int i = 1; i <= s; i++) ans += mat[i][s + 1];
    cout << ans << endl;
    for (int i = 1, id = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++, id++) {
        while (mat[id][s + 1]-- > 0) {
          cout << i << " " << j << endl;
        }
      }
    }
  }
  return 0;
}
