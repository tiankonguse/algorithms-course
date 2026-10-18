#include <bits/stdc++.h>
using namespace std;
// 固定例：题2 的 ShowDetails 十例 + 题4 确定性小例（复用题2 课程代码判定）
const int MAXN = 105;
double mat[MAXN][MAXN];
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
int signOf() {
  int sign = 1;
  for (int i = 1; i <= n; i++) {
    if (fabs(mat[i][i]) < sml && fabs(mat[i][n + 1]) >= sml) { sign = -1; break; }
    if (fabs(mat[i][i]) < sml) sign = 0;
  }
  return sign;
}
void setm(int nn, initializer_list<double> rows) {
  n = nn;
  memset(mat, 0, sizeof(mat));
  auto it = rows.begin();
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n + 1; j++) mat[i][j] = *it++;
}
int run() {
  gauss(n);
  return signOf();
}
int main() {
  int bad = 0;
  // 1 唯一解 (4,3,1)
  setm(3, {1,2,-1,9, 2,-1,2,7, 1,-2,2,0});
  if (run() != 1 || fabs(mat[1][4]-4) > 1e-6 || fabs(mat[2][4]-3) > 1e-6 || fabs(mat[3][4]-1) > 1e-6) { printf("例1 FAIL\n"); bad++; }
  // 2 矛盾
  setm(2, {1,1,3, 2,2,7});
  if (run() != -1) { printf("例2 FAIL\n"); bad++; }
  // 3 多解
  setm(2, {1,1,3, 2,2,6});
  if (run() != 0) { printf("例3 FAIL\n"); bad++; }
  // 4 表达式冗余唯一解（x1..x3 唯一，x4 系数全 0 → 自由元 → 严格判定多解）
  setm(4, {1,2,-1,0,9, 2,4,-2,0,18, 2,-1,2,0,7, 1,-2,2,0,0});
  if (run() != 0) { printf("例4 FAIL\n"); bad++; }
  // 5 表达式冗余矛盾
  setm(4, {1,2,-1,0,9, 2,4,-2,0,18, 2,-1,2,0,7, 4,-2,4,0,10});
  if (run() != -1) { printf("例5 FAIL\n"); bad++; }
  // 6 表达式冗余多解
  setm(4, {1,2,-1,0,9, 2,4,-2,0,18, 2,-1,2,0,7, 4,-2,4,0,14});
  if (run() != 0) { printf("例6 FAIL\n"); bad++; }
  // 7 表达式不足矛盾（补 0 行）
  setm(3, {1,2,-1,5, 2,4,-2,7, 0,0,0,0});
  if (run() != -1) { printf("例7 FAIL\n"); bad++; }
  // 8 表达式不足多解（补 0 行）
  setm(3, {1,2,-1,5, 2,2,-1,8, 0,0,0,0});
  if (run() != 0) { printf("例8 FAIL\n"); bad++; }
  // 9 0x1+2x2=3, 0x1+0x2=0 → 多解
  setm(2, {0,2,3, 0,0,0});
  if (run() != 0) { printf("例9 FAIL\n"); bad++; }
  // 10 a+b+c=5, 2a+b+c=7 → 多解
  setm(3, {1,1,1,5, 2,1,1,7, 0,0,0,0});
  if (run() != 0) { printf("例10 FAIL\n"); bad++; }
  // 11 a+b=5 → 多解
  setm(2, {1,1,5, 0,0,0});
  if (run() != 0) { printf("例11 FAIL\n"); bad++; }
  printf("fixed(题2 ShowDetails 11 例): %s\n", bad ? "FAIL" : "OK");

  // 题4 确定性小例：n=3, w=[2,5,3], 行4 组合改错
  // 行1={1}=2 行2={2}=5 行3={3}=3 行4={1,2}=7→改 9
  // 删行4 → 单件 (2,5,3) 合法 最重2；删行1 → 行2,3,4(9): x2=5,x3=3,x1+5=9→x1=4 (4,5,3) 合法 最重2；删行2 → x1=2,x3=3,x2+2=9→x2=7 (2,7,3) 合法 最重2；删行3 → x1=2,x2=5,x3+7=9→x3=2 (2,5,2) 合法 最重2 → times=4 → illegal
  int dataI[5][5] = {{0}, {0,1,0,0,2}, {0,0,1,0,5}, {0,0,0,1,3}, {0,1,1,0,9}};
  int times = 0, ans = 0;
  for (int k = 1; k <= 4; k++) {
    // 删除第 k 条
    memset(mat, 0, sizeof(mat));
    n = 3;
    int src = 0;
    for (int i = 1; i <= 3; i++) {
      src = (src + 1 == k) ? src + 2 : src + 1;
      for (int j = 1; j <= 4; j++) mat[i][j] = dataI[src][j];
    }
    gauss(3);
    int ok = 1;
    double maxv = 0; int maxt = 0, heav = 0;
    for (int i = 1; i <= 3; i++) {
      if (fabs(mat[i][i]) < sml) { ok = 0; break; }
      if (mat[i][n + 1] <= 0 || mat[i][n + 1] != (int)mat[i][n + 1]) { ok = 0; break; }
      if (maxv < mat[i][n + 1]) { maxv = mat[i][n + 1]; maxt = 1; heav = i; }
      else if (maxv == mat[i][n + 1]) maxt++;
    }
    if (ok && maxt == 1) { times++; ans = heav; }
  }
  if (times == 1) printf("题4 小例: %d\n", ans);
  else printf("题4 小例: illegal (times=%d)\n", times);
  return 0;
}
