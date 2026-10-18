#include <bits/stdc++.h>
using namespace std;
// 135 课 POJ2947 对拍：课程完整版 vs 独立参考（标准行阶梯+回代，mod 7）
int MOD = 7;
int inv[7];
void invs() { inv[1] = 1; for (int i = 2; i < MOD; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD); }
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

// ---- 课程版（照 t2.cpp）----
struct Course {
  int mat[302][302];
  int n, m, s;
  void prepare() { for (int i = 1; i <= s; i++) for (int j = 1; j <= s + 1; j++) mat[i][j] = 0; }
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
  int solve(int nn_, int mm_, vector<vector<int>>& rec, vector<int>& x) {
    n = nn_; m = mm_; s = max(n, m);
    prepare();
    for (int i = 1; i <= m; i++) {
      // rec[i-1] = {tool1..toolk, daydiff}
      for (size_t j = 0; j + 1 < rec[i - 1].size(); j++) {
        int tool = rec[i - 1][j];
        mat[i][tool] = (mat[i][tool] + 1) % MOD;
      }
      mat[i][s + 1] = rec[i - 1].back();
    }
    gauss(s);
    int sign = 1;
    for (int i = 1; i <= s; i++) {
      if (mat[i][i] == 0 && mat[i][s + 1] != 0) { sign = -1; break; }
      if (i <= n && mat[i][i] == 0) sign = 0;
    }
    if (sign == 1) {
      x.assign(n, 0);
      for (int i = 1; i <= n; i++) x[i - 1] = mat[i][s + 1];
    }
    return sign;
  }
};

// ---- 参考版：标准模7行阶梯（0-based）+ 回代 ----
struct Ref {
  int M[302][302]; // [0..m-1][0..n]（常数列 n）
  int refsolve(int nn, int mm, vector<vector<int>>& rec, vector<int>& x) {
    memset(M, 0, sizeof(M));
    for (int i = 0; i < mm; i++) {
      for (size_t j = 0; j + 1 < rec[i].size(); j++) {
        int tool = rec[i][j];
        M[i][tool - 1] = (M[i][tool - 1] + 1) % MOD;
      }
      M[i][nn] = rec[i].back();
    }
    int row = 0;
    vector<int> pivotCol(mm, -1);
    for (int col = 0; col < nn; col++) {
      int p = -1;
      for (int r = row; r < mm; r++) if (M[r][col] != 0) { p = r; break; }
      if (p == -1) continue;
      swap(M[row], M[p]);
      // 归一化
      int iv = inv[M[row][col]];
      for (int c = col; c <= nn; c++) M[row][c] = (M[row][c] * iv) % MOD;
      pivotCol[row] = col;
      for (int r = 0; r < mm; r++) {
        if (r != row && M[r][col] != 0) {
          int f = M[r][col];
          for (int c = col; c <= nn; c++) M[r][c] = ((M[r][c] - M[row][c] * f) % MOD + MOD) % MOD;
        }
      }
      row++;
    }
    // 矛盾：row..mm-1 行系数全 0 但常数 != 0
    for (int r = row; r < mm; r++) {
      bool allZero = true;
      for (int c = 0; c < nn; c++) if (M[r][c] != 0) { allZero = false; break; }
      if (allZero && M[r][nn] != 0) return -1;
    }
    if (row < nn) return 0;
    // 唯一解：回代（此时是约当型，直接取常数）
    x.assign(nn, 0);
    for (int r = 0; r < nn; r++) {
      x[pivotCol[r]] = M[r][nn];
    }
    return 1;
  }
};

mt19937 rng(2947);

int main() {
  invs();
  int T = 10000;
  int badSign = 0, badSol = 0, uni = 0, multi = 0, incons = 0;
  for (int t = 0; t < T; t++) {
    int n = 1 + rng() % 6;   // 工具数 1..6
    int mode = rng() % 3;    // 0=一致 1=一致 2=故意矛盾
    // 真天数
    vector<int> d(n);
    for (int i = 0; i < n; i++) d[i] = 3 + rng() % 7; // 3..9
    int m = n + (int)(rng() % 3); // n..n+2 条记录
    vector<vector<int>> rec;
    // 先造 m 条一致记录
    for (int i = 0; i < m; i++) {
      int k = 1 + rng() % n;
      vector<int> tools(k);
      int sum = 0;
      for (int j = 0; j < k; j++) {
        tools[j] = 1 + rng() % n;
        sum = (sum + d[tools[j] - 1]) % MOD;
      }
      // 固定 st=0，et = (st + sum - 1) mod 7 → daydiff = (et - st + 1) mod 7 = sum
      int st = rng() % 7;
      int et = ((st + sum - 1) % MOD + MOD) % MOD;
      vector<int> row = tools;
      row.push_back(((et - st + 1) % MOD + MOD) % MOD);
      rec.push_back(row);
    }
    if (mode == 2) {
      // 替换最后一条为矛盾记录：保持工具集，daydiff = (sum+1) mod 7
      auto last = rec.back();
      vector<int> tools2(last.begin(), last.end() - 1);
      int sum2 = 0;
      for (int tool : tools2) sum2 = (sum2 + d[tool - 1]) % MOD;
      int badDiff = (sum2 + 1) % MOD;
      int st = rng() % 7;
      int et = ((st + badDiff - 1) % MOD + MOD) % MOD;
      tools2.push_back(((et - st + 1) % MOD + MOD) % MOD);
      rec[m - 1] = tools2;
    }
    Course c;
    vector<int> xc;
    int sc = c.solve(n, m, rec, xc);
    Ref r;
    vector<int> xr;
    int sr = r.refsolve(n, m, rec, xr);
    if (sc != sr) {
      badSign++;
      if (badSign <= 5) {
        cout << "SIGN n=" << n << " m=" << m << " mode=" << mode << " course=" << sc << " ref=" << sr << endl;
      }
      continue;
    }
    if (sc == 1) {
      uni++;
      bool same = true;
      // 验证课程解与参考解 mod 7 一致，且课程解代入每条记录成立
      for (int i = 0; i < n; i++) {
        if ((xc[i] % MOD) != (xr[i] % MOD)) { same = false; }
      }
      // 代入验证（mode=2 且 m=1 时唯一解=矛盾行自身，代入同样成立）
      for (auto& r : rec) {
        int sum = 0;
        for (size_t j = 0; j + 1 < r.size(); j++) sum = (sum + xc[r[j] - 1]) % MOD;
        if (sum != r.back()) { same = false; }
      }
      if (!same) {
        badSol++;
        if (badSol <= 5) {
          cout << "SOL n=" << n << " m=" << m << " mode=" << mode << " d=";
          for (int v : d) cout << v << " ";
          cout << " course=";
          for (int v : xc) cout << v << " ";
          cout << " ref=";
          for (int v : xr) cout << v << " ";
          cout << " recs=";
          for (auto& r : rec) { cout << "["; for (int v : r) cout << v << ","; cout << "] "; }
          cout << endl;
        }
      }
    } else if (sc == 0) multi++;
    else incons++;
  }
  cout << "uni=" << uni << " multi=" << multi << " incons=" << incons << " badSign=" << badSign << " badSol=" << badSol << endl;
  cout << ((badSign == 0 && badSol == 0) ? "ALL OK" : "FAIL") << endl;
  return 0;
}
