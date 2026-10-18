// 题1 装备购买：课程（价格升序 + 实数消元贪心） vs 暴力（枚举子集：最大线性无关大小 + 最小价格）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Course {
  double sml = 1e-5;
  vector<vector<double>> mat;
  vector<int> basis;
  int n, m, cnt, cost;
  bool insert(int i) {
    for (int j = 1; j <= m; j++) {
      if (fabs(mat[i][j]) >= sml) {
        if (basis[j] == 0) { basis[j] = i; return true; }
        double rate = mat[i][j] / mat[basis[j]][j];
        for (int k = j; k <= m; k++) mat[i][k] -= rate * mat[basis[j]][k];
      }
    }
    return false;
  }
  void compute() {
    cnt = cost = 0;
    sort(mat.begin() + 1, mat.end(), [&](const vector<double>& a, const vector<double>& b) { return a[m + 1] < b[m + 1]; });
    basis.assign(m + 2, 0);   // 下标 j=1..m，m 可能 > n
    for (int i = 1; i <= n; i++) if (insert(i)) { cnt++; cost += (int)mat[i][m + 1]; }
  }
  pair<int,int> run(int nn, int mm, const vector<vector<int>>& attr, const vector<int>& price) {
    n = nn; m = mm;
    mat.assign(n + 1, vector<double>(m + 2, 0));
    for (int i = 1; i <= n; i++) { for (int j = 1; j <= m; j++) mat[i][j] = attr[i][j]; mat[i][m + 1] = price[i]; }
    compute();
    return {cnt, cost};
  }
};

int rankRows(vector<vector<ll>> rows, int m) {
  int r = 0;
  for (int j = 0; j < m; j++) {
    int piv = -1;
    for (int i = r; i < (int)rows.size(); i++) if (rows[i][j] != 0) { piv = i; break; }
    if (piv == -1) continue;
    swap(rows[r], rows[piv]);
    for (int i = 0; i < (int)rows.size(); i++) {
      if (i != r && rows[i][j] != 0) {
        ll a = rows[r][j], b = rows[i][j];
        ll g = std::gcd(a < 0 ? -a : a, b < 0 ? -b : b);
        a /= g; b /= g;
        for (int k = 0; k < m; k++) rows[i][k] = rows[i][k] * a - rows[r][k] * b;
      }
    }
    r++;
  }
  return r;
}

pair<int,int> brute(int n, int m, const vector<vector<int>>& attr, const vector<int>& price) {
  int bestCnt = 0, bestCost = INT_MAX;
  for (int mask = 1; mask < (1 << n); mask++) {
    vector<vector<ll>> rows;
    int costSum = 0;
    for (int i = 0; i < n; i++) if (mask >> i & 1) {
      vector<ll> row(m);
      for (int j = 0; j < m; j++) row[j] = attr[i + 1][j + 1];   // 属性列 1-based
      rows.push_back(row);
      costSum += price[i + 1];
    }
    int sz = rows.size();
    if (rankRows(rows, m) != sz) continue;
    if (sz > bestCnt || (sz == bestCnt && costSum < bestCost)) { bestCnt = sz; bestCost = costSum; }
  }
  if (bestCnt == 0) bestCost = 0;
  return {bestCnt, bestCost};
}

int main() {
  mt19937 rng(1234567);
  int fails = 0;
  for (int T = 0; T < 5000; T++) {
    int n = 1 + rng() % 6, m = 1 + rng() % 4;
    vector<vector<int>> attr(n + 1, vector<int>(m + 1, 0));
    vector<int> price(n + 1);
    for (int i = 1; i <= n; i++) { for (int j = 1; j <= m; j++) attr[i][j] = rng() % 4; price[i] = 1 + rng() % 9; }
    auto c = Course().run(n, m, attr, price);
    auto b = brute(n, m, attr, price);
    if (c.first != b.first || c.second != b.second) {
      fails++;
      printf("MISMATCH T=%d 课程=(%d,%d) 暴力=(%d,%d) n=%d m=%d\n", T, c.first, c.second, b.first, b.second, n, m);
      for (int i = 1; i <= n; i++) { printf("  物品%d 属性:", i); for (int j = 1; j <= m; j++) printf(" %d", attr[i][j]); printf(" 价格=%d\n", price[i]); }
      if (fails > 3) return 1;
    }
  }
  if (fails) return 1;
  printf("5000 组 ALL OK\n");
  return 0;
}
