#include <bits/stdc++.h>
#include "fraclib.h"
using namespace std;

// ===== 课程题1（P3389）：区分「有唯一解 / 没有」 =====
const int MAXN = 105;
double mat[MAXN][MAXN];
double sml = 1e-7;
int n;
void swapRows(int a, int b) {
  for (int k = 1; k <= n + 1; k++) swap(mat[a][k], mat[b][k]);
}
int gauss1(int n) {
  for (int i = 1; i <= n; i++) {
    int max = i;
    for (int j = i + 1; j <= n; j++)
      if (fabs(mat[j][i]) > fabs(mat[max][i])) max = j;
    swapRows(i, max);
    if (fabs(mat[i][i]) < sml) return 0;
    double tmp = mat[i][i];
    for (int j = i; j <= n + 1; j++) mat[i][j] /= tmp;
    for (int j = 1; j <= n; j++) {
      if (i != j) {
        double rate = mat[j][i] / mat[i][i];
        for (int k = i; k <= n + 1; k++) mat[j][k] -= mat[i][k] * rate;
      }
    }
  }
  return 1;
}

mt19937 rng(12345);
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
    int course = gauss1(n);
    if (course == 0 && ref == 1) {
      printf("MISMATCH t=%d n=%d: course=NoSolution ref=unique\n", t, n);
      bad++;
    } else if (course == 1) {
      if (ref != 1) {
        printf("MISMATCH t=%d n=%d: course=unique ref=%d\n", t, n, ref);
        bad++;
      } else {
        for (int i = 1; i <= n; i++) {
          double cv = mat[i][n + 1], rv = (double)sol[i].toD();
          if (fabs(cv - rv) > 1e-6) {
            printf("MISMATCH t=%d n=%d x%d: course=%.10f ref=%.10f\n", t, n, i, cv, rv);
            bad++;
            break;
          }
        }
      }
    }
    if (bad > 5) break;
  }
  printf("t1: %d groups, bad=%d %s\n", T, bad, bad ? "FAIL" : "OK");
  return 0;
}
