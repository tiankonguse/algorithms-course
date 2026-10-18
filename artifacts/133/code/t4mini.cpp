#include <bits/stdc++.h>
using namespace std;
const int MAXN = 105;
double mat[MAXN][MAXN];
double sml = 1e-7;
int n;
void swapRows(int a, int b) { for (int k = 1; k <= n + 1; k++) swap(mat[a][k], mat[b][k]); }
void gauss(int n) {
  for (int i = 1; i <= n; i++) {
    int max = i;
    for (int j = 1; j <= n; j++) {
      if (j < i && fabs(mat[j][j]) >= sml) continue;
      if (fabs(mat[j][i]) > fabs(mat[max][i])) max = j;
    }
    swapRows(i, max);
    if (fabs(mat[i][i]) >= sml) {
      double tmp = mat[i][i];
      for (int j = i; j <= n + 1; j++) mat[i][j] /= tmp;
      for (int j = 1; j <= n; j++) {
        if (i != j) {
          double rate = mat[j][i] / mat[i][i];
          for (int k = i; k <= n + 1; k++) mat[j][k] -= mat[i][k] * rate;
        }
      }
    }
  }
}
int main() {
  int dataI[5][5] = {{0}, {0,1,0,0,2}, {0,0,1,0,5}, {0,0,0,1,3}, {0,1,1,0,9}};
  for (int k = 1; k <= 4; k++) {
    memset(mat, 0, sizeof(mat));
    n = 3;
    int src = 0;
    for (int i = 1; i <= 3; i++) {
      src = (src + 1 == k) ? src + 2 : src + 1;
      for (int j = 1; j <= 4; j++) mat[i][j] = dataI[src][j];
    }
    printf("删第%d条 矩阵:\n", k);
    for (int i = 1; i <= 3; i++) { for (int j = 1; j <= 4; j++) printf("%.0f ", mat[i][j]); printf("\n"); }
    gauss(3);
    int ok = 1; double maxv = 0; int maxt = 0, heav = 0;
    for (int i = 1; i <= 3; i++) {
      if (fabs(mat[i][i]) < sml) { ok = 0; break; }
      if (mat[i][n + 1] <= 0 || mat[i][n + 1] != (int)mat[i][n + 1]) { ok = 0; break; }
      if (maxv < mat[i][n + 1]) { maxv = mat[i][n + 1]; maxt = 1; heav = i; }
      else if (maxv == mat[i][n + 1]) maxt++;
    }
    printf("  消元后: x1=%.6f x2=%.6f x3=%.6f -> ok=%d maxt=%d heav=%d\n", mat[1][4], mat[2][4], mat[3][4], ok, maxt, heav);
  }
  return 0;
}
