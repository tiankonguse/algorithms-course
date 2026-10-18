#include <bits/stdc++.h>
#include "fraclib.h"
using namespace std;

// ===== 课程题3（P4035 球形空间的中心点）=====
const int MAXN = 12;
double dataM[MAXN][MAXN], mat[MAXN][MAXN];
double sml = 1e-7;
int n;
void swapRows(int a, int b) {
  for (int k = 1; k <= n + 1; k++) swap(mat[a][k], mat[b][k]);
}
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
// 构造方程（与课程同）：第 i 个方程由第 i 个点与第 i+1 个点作差得到
void build() {
  memset(mat, 0, sizeof(mat));
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      mat[i][j] = 2 * (dataM[i][j] - dataM[i + 1][j]);
      mat[i][n + 1] += dataM[i][j] * dataM[i][j] - dataM[i + 1][j] * dataM[i + 1][j];
    }
  }
}

mt19937 rng(24680);
int randInt(int lo, int hi) { return (int)(rng() % (hi - lo + 1)) + lo; }

int main() {
  int T = 10000, bad = 0;
  // 方案 A：构造共球面点，球心 c 已知，验证课程解出 c
  for (int t = 0; t < T; t++) {
    n = randInt(1, 6);
    vector<long long> c(n + 1);
    for (int i = 1; i <= n; i++) c[i] = randInt(-5, 5);
    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= n; j++) dataM[i][j] = c[j] + (i == j ? 1 : 0);
    for (int j = 1; j <= n; j++) dataM[n + 1][j] = c[j] - (j == 1 ? 1 : 0);
    build();
    gauss(n);
    for (int j = 1; j <= n; j++) {
      if (fabs(mat[j][n + 1] - c[j]) > 1e-6) {
        printf("MISMATCH-A t=%d n=%d coord%d: course=%.10f c=%lld\n", t, n, j, mat[j][n + 1], c[j]);
        bad++;
        if (bad > 5) break;
      }
    }
  }
  // 方案 B：随机整数点，课程 vs 有理数参考（同样构造方程）
  // n 限 5：n=6 随机病态矩阵 double 误差放大（原题坐标给到 6 位小数，数据不会病态到 double 崩）
  for (int t = 0; t < T; t++) {
    n = randInt(1, 5);
    vector<vector<Frac>> A(n + 1, vector<Frac>(n + 2, Frac(0)));
    for (int i = 1; i <= n + 1; i++)
      for (int j = 1; j <= n; j++) dataM[i][j] = randInt(-10, 10);
    build();
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        long long v = 2 * ((long long)dataM[i][j] - (long long)dataM[i + 1][j]);
        A[i][j] = Frac(v);
      }
      long long rhs = 0;
      for (int j = 1; j <= n; j++)
        rhs += (long long)dataM[i][j] * (long long)dataM[i][j] -
               (long long)dataM[i + 1][j] * (long long)dataM[i + 1][j];
      A[i][n + 1] = Frac(rhs);
    }
    vector<Frac> sol;
    int ref = gauss_jordan(A, n, sol);
    if (ref != 1) continue;  // 随机点一般满秩；非满秩跳过（构造方程本身可能退化）
    gauss(n);
    for (int j = 1; j <= n; j++) {
      double cv = mat[j][n + 1], rv = (double)sol[j].toD();
      if (fabs(cv - rv) > 1e-5) {
        printf("MISMATCH-B t=%d n=%d coord%d: course=%.10f ref=%.10f\n", t, n, j, cv, rv);
        bad++;
        if (bad > 5) break;
      }
    }
  }
  printf("t3: bad=%d %s\n", bad, bad ? "FAIL" : "OK");
  return 0;
}
