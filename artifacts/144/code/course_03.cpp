#include <bits/stdc++.h>
using namespace std;
const int MAXV = 2000, MAXN = 2002;
int c[MAXN][MAXN], f[MAXN][MAXN], sum[MAXN][MAXN];
int main() {
  int t, k; cin >> t >> k;
  for (int i = 0; i <= MAXV; i++) {
    c[i][0] = 1;
    for (int j = 1; j <= i; j++) c[i][j] = (c[i-1][j] + c[i-1][j-1]) % k;
  }
  for (int i = 1; i <= MAXV; i++)
    for (int j = 1; j <= i; j++) f[i][j] = c[i][j] % k == 0 ? 1 : 0;
  for (int i = 2; i <= MAXV; i++) {
    for (int j = 1; j <= i; j++)
      sum[i][j] = sum[i][j-1] + sum[i-1][j] - sum[i-1][j-1] + f[i][j];
    sum[i][i+1] = sum[i][i];
  }
  while (t--) {
    int n, m; cin >> n >> m;
    cout << (m > n ? sum[n][n] : sum[n][m]) << "\n";
  }
}
