#include <bits/stdc++.h>
#include "fraclib.h"
using namespace std;

// ===== 课程题2（P2455）：严格区分 矛盾 / 多解 / 唯一解 =====
const int MAXN = 55;
double mat[MAXN][MAXN];
double sml = 1e-7;
int n;
void swapRows(int a, int b) {
  for (int k = 1; k <= n + 1; k++) swap(mat[a][k], mat[b][k]);
}
void gauss2(int n) {
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
// 1 唯一解 / 0 多解 / -1 矛盾
int signOf() {
  int sign = 1;
  for (int i = 1; i <= n; i++) {
    if (fabs(mat[i][i]) < sml && fabs(mat[i][n + 1]) >= sml) { sign = -1; break; }
    if (fabs(mat[i][i]) < sml) sign = 0;
  }
  return sign;
}

mt19937 rng(9876);
int randInt(int lo, int hi) { return (int)(rng() % (hi - lo + 1)) + lo; }

int main() {
  int T = 20000, bad = 0;
  for (int t = 0; t < T; t++) {
    n = randInt(1, 6);
    vector<vector<Frac>> A(n + 1, vector<Frac>(n + 2));
    memset(mat, 0, sizeof(mat));
    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= n + 1; j++) {
        long long v = randInt(-5, 5);
        if (j == n + 1) v = randInt(-10, 10);
        mat[i][j] = v;
        A[i][j] = Frac(v);
      }
    vector<Frac> sol;
    int ref = gauss_jordan(A, n, sol);
    gauss2(n);
    int course = signOf();
    if (course != ref) {
      printf("MISMATCH t=%d n=%d: course=%d ref=%d\n", t, n, course, ref);
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n + 1; j++) printf("%d ", (int)mat[i][j]);
        printf("\n");
      }
      bad++;
    } else if (ref == 1) {
      for (int i = 1; i <= n; i++) {
        double cv = mat[i][n + 1], rv = (double)sol[i].toD();
        if (fabs(cv - rv) > 1e-6) {
          printf("MISMATCH t=%d n=%d x%d: course=%.10f ref=%.10f\n", t, n, i, cv, rv);
          bad++;
          break;
        }
      }
    }
    if (bad > 5) break;
  }
  printf("t2: %d groups, bad=%d %s\n", T, bad, bad ? "FAIL" : "OK");
  return 0;
}
