#include <bits/stdc++.h>
using namespace std;

// ===== 课程版：Code03_AlienInsectLegs 改写（位图）=====
const int BIT = 64;
const int MAXN = 2002;
const int MAXM = MAXN / BIT + 1;
long long mat[MAXN][MAXM];
int n, m, s;
int need;

void setbit(int row, int col, int v) {
  if (v == 0) mat[row][col / BIT] &= ~(1LL << (col % BIT));
  else mat[row][col / BIT] |= 1LL << (col % BIT);
}
int getbit(int row, int col) {
  return ((mat[row][col / BIT] >> (col % BIT)) & 1) == 1 ? 1 : 0;
}
void eor(int row1, int row2, int bits) {
  for (int k = 0; k <= bits / BIT; k++) mat[row2][k] ^= mat[row1][k];
}
void swaprows(int a, int b) {
  for (int k = 0; k < MAXM; k++) swap(mat[a][k], mat[b][k]);
}

void gauss_course(int gn) {
  need = 0;
  for (int i = 1; i <= gn; i++) {
    for (int j = i; j <= gn; j++) {
      if (getbit(j, i) == 1) {
        swaprows(i, j);
        need = max(need, j);
        break;
      }
    }
    if (getbit(i, i) == 0) return;
    for (int j = 1; j <= gn; j++) {
      if (i != j && getbit(j, i) == 1) eor(i, j, n + 2);
    }
  }
}

// 一次性构造课程输入
void build_course(vector<string>& rows, vector<int>& b, int nn, int mm) {
  n = nn; m = mm; s = max(n, m);
  memset(mat, 0, sizeof(mat));
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) setbit(i, j, rows[i-1][j-1] - '0');
    setbit(i, s + 1, b[i-1]);
  }
  gauss_course(s);
}

// 输出：need（或 -1 表示 Cannot Determine）、解向量
pair<int, vector<int>> course_solve(vector<string>& rows, vector<int>& b, int nn, int mm) {
  build_course(rows, b, nn, mm);
  int sign = 1;
  for (int i = 1; i <= n; i++) if (getbit(i, i) == 0) { sign = 0; break; }
  if (sign == 0) return {-1, {}};
  vector<int> x(n+1);
  for (int i = 1; i <= n; i++) x[i] = getbit(i, s + 1);
  return {need, x};
}

// ===== 参考版：前 k 条记录标准消元，判满秩；取最早 k = 参考 need =====
// 与课程一致：找主元 j 从 i..k，只对前 n 行消元
int matref[2010][2010];
int ref_solve(vector<string>& rows, vector<int>& b, int nn, int mm, int k, vector<int>& xout) {
  n = nn;
  memset(matref, 0, sizeof(matref));
  for (int i = 1; i <= k; i++) {
    for (int j = 1; j <= n; j++) matref[i][j] = rows[i-1][j-1] - '0';
    matref[i][n+1] = b[i-1];
  }
  for (int i = 1; i <= n; i++) {
    int p = -1;
    for (int j = i; j <= k; j++) if (matref[j][i]) { p = j; break; }
    if (p == -1) return 0;  // 该列无主元 -> 不满秩
    for (int j = 1; j <= n+1; j++) swap(matref[i][j], matref[p][j]);
    for (int j = 1; j <= k; j++) {
      if (j != i && matref[j][i]) {
        for (int c = i; c <= n+1; c++) matref[j][c] ^= matref[i][c];
      }
    }
  }
  // 唯一解
  for (int i = 1; i <= n; i++) xout[i] = matref[i][n+1];
  return 1;
}

// ===== 随机生成：n 条满秩基础记录 + 冗余，随机打乱 =====
mt19937 rng(20261016);

int main() {
  int tested = 0;
  while (tested < 3000) {
    n = 2 + rng() % 7;            // 2..8
    m = n + rng() % 5;            // n..n+4
    // 随机满秩 A（n x n）逐行生成保证独立
    int A[20][20];
    vector<int> piv(n+1, 0);
    memset(A, 0, sizeof(A));
    // 简化：用「上三角 + 行变换」构造可逆——直接随机尝试，用高斯消元检查秩
    for (int r = 1; r <= n; r++) {
      // 随机行，保证与已有行线性无关（检查）
      while (true) {
        for (int c = 1; c <= n; c++) A[r][c] = rng() & 1;
        // 检查 rank（对前 r 行）
        int B[20][20]; memset(B, 0, sizeof(B));
        for (int i = 1; i <= r; i++) for (int c = 1; c <= n; c++) B[i][c] = A[i][c];
        int rk = 0;
        for (int c = 1; c <= n && rk < r; c++) {
          int p = -1;
          for (int i = rk+1; i <= r; i++) if (B[i][c]) { p = i; break; }
          if (p == -1) continue;
          rk++;
          for (int j = 1; j <= n; j++) swap(B[rk][j], B[p][j]);
          for (int i = rk+1; i <= r; i++) if (B[i][c]) for (int j = c; j <= n; j++) B[i][j] ^= B[rk][j];
        }
        if (rk == r) break;
      }
    }
    // 随机解 x
    vector<int> x(n+1);
    for (int i = 1; i <= n; i++) x[i] = rng() & 1;
    vector<int> bx(n+1);
    for (int i = 1; i <= n; i++) {
      int t = 0;
      for (int j = 1; j <= n; j++) if (A[i][j] && x[j]) t ^= 1;
      bx[i] = t;
    }
    // 全部 m 条记录：前 n 条是基础记录，后 m-n 条是冗余（取 A 行的非零线性组合，
    // 常数取对应 bx 组合，保证与 x 一致；组合行是 A 行空间的元素，不补秩）
    vector<string> rows(m);
    vector<int> b(m);
    for (int i = 1; i <= n; i++) {
      string r;
      for (int j = 1; j <= n; j++) r += char('0' + A[i][j]);
      rows[i-1] = r;
      b[i-1] = bx[i];
    }
    for (int i = n+1; i <= m; i++) {
      // v[j]=1 表示取第 j 条基础记录，冗余行 = 这些行的异或，常数 = bx 对应异或
      vector<int> v(n+1);
      int nonz = 0;
      for (int j = 1; j <= n; j++) { v[j] = rng() & 1; nonz += v[j]; }
      if (nonz == 0 || nonz == n) { i--; continue; }  // 零行或全取（=所有行异或可能非零，也合法，但跳过简单化）
      string r; int t = 0;
      for (int c = 1; c <= n; c++) {
        int bit = 0;
        for (int j = 1; j <= n; j++) if (v[j] && A[j][c]) bit ^= 1;
        r += char('0' + bit);
      }
      for (int j = 1; j <= n; j++) if (v[j]) t ^= bx[j];
      rows[i-1] = r;
      b[i-1] = t;
    }
    // 随机打乱记录顺序
    for (int i = m-1; i > 0; i--) {
      int j = rng() % (i+1);
      swap(rows[i], rows[j]); swap(b[i], b[j]);
    }
    // 课程解
    auto cs = course_solve(rows, b, n, m);
    if (cs.first == -1) {
      printf("ERR: 课程说 Cannot Determine，但构造保证满秩\n"); return 1;
    }
    // 参考 need：最早前缀满秩
    int refneed = -1;
    for (int k = 1; k <= m; k++) {
      vector<int> xx(n+1);
      if (ref_solve(rows, b, n, m, k, xx)) { refneed = k; break; }
    }
    if (refneed == -1) { printf("ERR: 参考找不到满秩前缀\n"); return 1; }
    if (cs.first != refneed) {
      printf("MISMATCH-need n=%d m=%d course=%d ref=%d\n", n, m, cs.first, refneed);
      for (int i = 0; i < m; i++) printf("%s %d\n", rows[i].c_str(), b[i]);
      return 1;
    }
    // 参考解（用前 refneed 条）
    vector<int> xx(n+1);
    if (!ref_solve(rows, b, n, m, refneed, xx)) { printf("ERR ref solve\n"); return 1; }
    for (int i = 1; i <= n; i++) {
      if (cs.second[i] != xx[i] || cs.second[i] != x[i]) {
        printf("MISMATCH-x n=%d m=%d i=%d course=%d ref=%d true=%d\n", n, m, i, cs.second[i], xx[i], x[i]);
        for (int r = 0; r < m; r++) printf("%s %d\n", rows[r].c_str(), b[r]);
        return 1;
      }
    }
    tested++;
  }
  printf("t3 OK 3000 groups\n");

  // 固定例 1：PPT 例子（n=3 唯一解）
  {
    vector<string> rows = {"110", "101", "011"};  // 全 1 常数的 x1^x2=1, x1^x3=1, x2^x3=1 -> 矛盾？
    vector<int> b = {1, 1, 1};
    // 这其实是 ShowDetails 的矛盾例（无解），构造数据不含，跳过
  }
  // 固定例 2：n=3 三角满秩：x1=0, x2=1, x3=1
  {
    n = 3; m = 3;
    vector<string> rows = {"100", "110", "111"};
    vector<int> b = {0, 1, 0};  // x1=0; x1^x2=1 -> x2=1; x1^x2^x3=0 -> x3=1
    auto cs = course_solve(rows, b, n, m);
    printf("fix2 n=3 三角 need=%d x=(%d,%d,%d) (期望 need=3 x=(0,1,1))\n",
           cs.first, cs.second[1], cs.second[2], cs.second[3]);
  }
  // 固定例 3：冗余让 need 变小——第 3 条记录能定 x3
  {
    n = 3; m = 3;
    vector<string> rows = {"110", "101", "001"};
    vector<int> b = {1, 1, 1};  // x1^x2=1, x1^x3=1, x3=1 -> x1=0,x2=1,x3=1 唯一
    auto cs = course_solve(rows, b, n, m);
    printf("fix3 n=3 need=%d x=(%d,%d,%d) (期望 need=3 x=(0,1,1))\n",
           cs.first, cs.second[1], cs.second[2], cs.second[3]);
  }
  // 固定例 4：need 用不到最后一条（4 条记录，前 3 条满秩）
  {
    n = 3; m = 4;
    vector<string> rows = {"100", "010", "001", "111"};
    vector<int> b = {1, 0, 1, 0};  // x1=1,x2=0,x3=1；第 4 条冗余 1^0^1=0 ✓
    auto cs = course_solve(rows, b, n, m);
    printf("fix4 n=3 m=4 need=%d x=(%d,%d,%d) (期望 need=3 x=(1,0,1))\n",
           cs.first, cs.second[1], cs.second[2], cs.second[3]);
  }
  return 0;
}
