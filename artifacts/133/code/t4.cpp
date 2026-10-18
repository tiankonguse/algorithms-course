#include <bits/stdc++.h>
#include "fraclib.h"
using namespace std;

// ===== 课程题4（P5027 有一次错误称重求最重物品）=====
// v1：double 高斯消元 + 判整；v2：long 整数辗转消元 + 回代
const int MAXN = 105;
int dataI[MAXN][MAXN];  // dataI[i][1..n]=物品标记, dataI[i][n+1]=总重
double mat[MAXN][MAXN];
long lmat[MAXN][MAXN];
double sml = 1e-7;
int n;

void swapRows(int a, int b) {
  for (int k = 1; k <= n + 1; k++) swap(mat[a][k], mat[b][k]);
}
void gaussD(int n) {  // 题2 版模板
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
// v1 check：唯一正整数解 + 最重唯一，返回最重编号，否则 0
int checkD() {
  gaussD(n);
  double maxv = 0;
  int maxt = 0, ans = 0;
  for (int i = 1; i <= n; i++) {
    if (fabs(mat[i][i]) < sml) return 0;
    if (mat[i][n + 1] <= 0 || mat[i][n + 1] != (int)mat[i][n + 1]) return 0;
    if (maxv < mat[i][n + 1]) { maxv = mat[i][n + 1]; maxt = 1; ans = i; }
    else if (maxv == mat[i][n + 1]) maxt++;
  }
  if (maxt > 1) return 0;
  return ans;
}
void swapL(int a, int b) {
  for (int k = 1; k <= n + 1; k++) swap(lmat[a][k], lmat[b][k]);
}
void gaussL(int n) {  // v2：整数辗转消元，只消下方，之后回代
  for (int i = 1; i <= n; i++) {
    int max = i;
    for (int j = 1; j <= n; j++) {
      if (j < i && lmat[j][j] != 0) continue;
      if (llabs(lmat[j][i]) > llabs(lmat[max][i])) max = j;
    }
    swapL(i, max);
    if (lmat[i][i] == 0) return;
    for (int j = i + 1; j <= n; j++) {
      while (lmat[j][i] != 0) {
        long rate = lmat[i][i] / lmat[j][i];
        if (rate != 0) {
          for (int k = i; k <= n + 1; k++) lmat[i][k] -= lmat[j][k] * rate;
        }
        swapL(i, j);
      }
    }
  }
}
// v2 check：回代求精确整数解
int checkL() {
  gaussL(n);
  for (int i = n; i >= 1; i--) {
    if (lmat[i][i] == 0) return 0;
    for (int j = i + 1; j <= n; j++) lmat[i][n + 1] -= lmat[i][j] * lmat[j][n + 1];
    if (lmat[i][n + 1] % lmat[i][i] != 0) return 0;
    lmat[i][n + 1] /= lmat[i][i];
  }
  long maxv = 0;
  int maxt = 0, ans = 0;
  for (int i = 1; i <= n; i++) {
    if (lmat[i][i] == 0) return 0;
    if (lmat[i][n + 1] <= 0) return 0;
    if (maxv < lmat[i][n + 1]) { maxv = lmat[i][n + 1]; maxt = 1; ans = i; }
    else if (maxv == lmat[i][n + 1]) maxt++;
  }
  if (maxt > 1) return 0;
  return ans;
}
void swapData(int i, int j) {
  for (int k = 1; k <= n + 1; k++) swap(dataI[i][k], dataI[j][k]);
}
// 枚举删除第 k 条后解 n 个方程；返回 0 或最重编号
int solve(int k) {
  swapData(k, n + 1);
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n + 1; j++) {
      mat[i][j] = dataI[i][j];
      lmat[i][j] = dataI[i][j];
    }
  swapData(k, n + 1);
  return checkD();  // v1
}
int solveL(int k) {
  swapData(k, n + 1);
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n + 1; j++) lmat[i][j] = dataI[i][j];
  swapData(k, n + 1);
  return checkL();  // v2
}
// 参考：有理数精确版（枚举删除 + 精确判定）
int solveRef(int k, vector<vector<int>>& D, int nn) {
  vector<vector<Frac>> A(nn + 1, vector<Frac>(nn + 2, Frac(0)));
  for (int i = 1; i <= nn; i++) {
    int src = i < k ? i : i + 1;  // 排除第 k 行，与课程 swapData 语义一致
    for (int j = 1; j <= nn + 1; j++) A[i][j] = Frac(D[src][j]);
  }
  vector<Frac> sol;
  if (gauss_jordan(A, nn, sol) != 1) return 0;
  long long maxv = 0;
  int maxt = 0, ans = 0;
  for (int i = 1; i <= nn; i++) {
    if (sol[i].q != 1) return 0;         // 非整数
    if (sol[i].p <= 0) return 0;         // 非正整数
    if (maxv < sol[i].p) { maxv = sol[i].p; maxt = 1; ans = i; }
    else if (maxv == sol[i].p) maxt++;
  }
  if (maxt > 1) return 0;
  return ans;
}

mt19937 rng(13579);
int randInt(int lo, int hi) { return (int)(rng() % (hi - lo + 1)) + lo; }

int main() {
  int T = 3000, badD = 0, badL = 0;
  for (int t = 0; t < T; t++) {
    n = randInt(1, 6);
    vector<int> w(n + 1);
    int maxv = 0, heavy = 1;
    for (int i = 1; i <= n; i++) {
      w[i] = randInt(1, 15);
      if (w[i] > maxv) { maxv = w[i]; heavy = i; }
    }
    // 构造 n+1 条称重：前 n 条单件 i，第 n+1 条组合（含多个物品）
    memset(dataI, 0, sizeof(dataI));
    for (int i = 1; i <= n; i++) { dataI[i][i] = 1; dataI[i][n + 1] = w[i]; }
    int cm = n >= 2 ? randInt(2, n) : 1;
    int combSum = 0;
    vector<int> picked(n + 1, 0);
    for (int c = 1; c <= cm; c++) {
      int it;
      do { it = randInt(1, n); } while (picked[it]);
      picked[it] = 1;
      dataI[n + 1][it] = 1;
      combSum += w[it];
    }
    dataI[n + 1][n + 1] = combSum;
    // 随机选 1~2 条改成错误（改总重）
    int errCnt = randInt(1, 2);
    vector<int> errIdx;
    for (int e = 0; e < errCnt; e++) {
      int k = randInt(1, n + 1);
      if (find(errIdx.begin(), errIdx.end(), k) == errIdx.end()) errIdx.push_back(k);
    }
    for (int k : errIdx) {
      int delta = randInt(1, 5) * (randInt(0, 1) ? 1 : -1);
      dataI[k][n + 1] += delta;
    }
    // 拷贝一份做参考
    vector<vector<int>> D(n + 2, vector<int>(n + 2, 0));
    for (int i = 1; i <= n + 1; i++)
      for (int j = 1; j <= n + 1; j++) D[i][j] = dataI[i][j];
    // 课程 v1 / v2 / 参考 都枚举删除
    int timesD = 0, ansD = 0, timesL = 0, ansL = 0, timesRef = 0, ansRef = 0;
    for (int k = 1; k <= n + 1; k++) {
      int c1 = solve(k);
      if (c1) { timesD++; ansD = c1; }
      int c2 = solveL(k);
      if (c2) { timesL++; ansL = c2; }
      int c3 = solveRef(k, D, n);
      if (c3) { timesRef++; ansRef = c3; }
    }
    int outD = (timesD == 1) ? ansD : 0;
    int outL = (timesL == 1) ? ansL : 0;
    int outRef = (timesRef == 1) ? ansRef : 0;
    if (outD != outRef) {
      printf("MISMATCH-D t=%d n=%d: courseD=%d ref=%d times(%d,%d)\n", t, n, outD, outRef, timesD, timesRef);
      badD++;
    }
    if (outL != outRef) {
      printf("MISMATCH-L t=%d n=%d: courseL=%d ref=%d times(%d,%d)\n", t, n, outL, outRef, timesL, timesRef);
      badL++;
    }
    if (badD > 5 && badL > 5) break;
  }
  printf("t4: %d groups, badD=%d badL=%d %s\n", T, badD, badL,
         (badD || badL) ? "FAIL" : "OK");
  return 0;
}
