#include <bits/stdc++.h>
using namespace std;
// 135 课 HDU5755 对拍：课程简化版 vs 全枚举合法解（s<=6 全枚举；s>=7 只验合法性）
int MOD = 3;
int inv[3];
void invs() { inv[1] = 1; for (int i = 2; i < MOD; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD); }
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

struct Course {
  int mat[101][101];
  int n, m, s;
  void prepare() {
    for (int i = 1; i <= s; i++) for (int j = 1; j <= s + 1; j++) mat[i][j] = 0;
    int dir[] = {0, -1, 0, 1, 0};
    int cur, row, col;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
      cur = i * m + j + 1;
      mat[cur][cur] = 2;
      for (int d = 0; d <= 3; d++) {
        row = i + dir[d]; col = j + dir[d + 1];
        if (row >= 0 && row < n && col >= 0 && col < m) mat[cur][row * m + col + 1] = 1;
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
            if (j < i && mat[j][j] != 0) mat[j][j] = (mat[j][j] * a) % MOD;
            for (int k = i; k <= nn + 1; k++) mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
          }
        }
      }
    }
    for (int i = 1; i <= nn; i++) if (mat[i][i] != 0) mat[i][nn + 1] = (mat[i][nn + 1] * inv[mat[i][i]]) % MOD;
  }
  vector<int> solve(int nn_, int mm_, const vector<int>& init) {
    n = nn_; m = mm_; s = n * m;
    prepare();
    for (int i = 1; i <= s; i++) mat[i][s + 1] = (3 - init[i - 1]) % MOD;
    gauss(s);
    vector<int> x(s, 0);
    for (int i = 1; i <= s; i++) x[i - 1] = mat[i][s + 1];
    return x;
  }
};

mt19937 rng(135135);
int n, m, s;
int A[10][10];
vector<int> apply(const vector<int>& x) {
  vector<int> b(s, 0);
  for (int i = 0; i < s; i++) {
    int sum = 0;
    for (int j = 0; j < s; j++) sum = (sum + A[i + 1][j + 1] * x[j]) % MOD;
    b[i] = sum;
  }
  return b;
}
bool legal(const vector<int>& x, const vector<int>& b) {
  auto bb = apply(x);
  for (int i = 0; i < s; i++) if (bb[i] != b[i]) return false;
  return true;
}

int main() {
  invs();
  int T = 20000;
  int bad = 0, badEnum = 0;
  for (int t = 0; t < T; t++) {
    n = 1 + rng() % 3;
    m = 1 + rng() % 3;
    s = n * m;
    memset(A, 0, sizeof(A));
    int dir[] = {0, -1, 0, 1, 0};
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
      int cur = i * m + j + 1;
      A[cur][cur] = 2;
      for (int d = 0; d <= 3; d++) {
        int row = i + dir[d], col = j + dir[d + 1];
        if (row >= 0 && row < n && col >= 0 && col < m) A[cur][row * m + col + 1] = 1;
      }
    }
    vector<int> xtrue(s);
    for (int i = 0; i < s; i++) xtrue[i] = rng() % MOD;
    vector<int> b = apply(xtrue);
    vector<int> init(s);
    for (int i = 0; i < s; i++) init[i] = (3 - b[i]) % MOD;
    Course c;
    vector<int> xc = c.solve(n, m, init);
    if (!legal(xc, b)) { bad++; if (bad <= 3) cout << "BAD course sol n=" << n << " m=" << m << endl; continue; }
    // s<=6 才全枚举合法解集（3^s<=729）
    if (s <= 6) {
      bool any = false;
      long long total = 1; for (int i = 0; i < s; i++) total *= MOD;
      for (long long mask = 0; mask < total; mask++) {
        vector<int> x(s);
        long long mm = mask;
        for (int i = 0; i < s; i++) { x[i] = mm % MOD; mm /= MOD; }
        if (legal(x, b)) { any = true; break; }
      }
      if (!any) { badEnum++; cout << "NO legal sol n=" << n << " m=" << m << endl; }
    }
  }
  cout << "bad=" << bad << " badEnum=" << badEnum << endl;
  cout << (bad == 0 && badEnum == 0 ? "ALL OK" : "FAIL") << endl;
  return 0;
}
