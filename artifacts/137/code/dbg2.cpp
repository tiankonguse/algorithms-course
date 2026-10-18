#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
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
      for (int j = 0; j < m; j++) row[j] = attr[i + 1][j];
      rows.push_back(row);
      costSum += price[i + 1];
    }
    int sz = rows.size();
    int rk = rankRows(rows, m);
    if (mask == (1 << n) - 1) printf("全选 mask=%d sz=%d rank=%d\n", mask, sz, rk);
    if (rk != sz) continue;
    if (sz > bestCnt || (sz == bestCnt && costSum < bestCost)) { bestCnt = sz; bestCost = costSum; }
  }
  if (bestCnt == 0) bestCost = 0;
  return {bestCnt, bestCost};
}
int main() {
  int n = 4, m = 4;
  vector<vector<int>> attr(n + 1, vector<int>(m + 1));
  vector<int> price(n + 1);
  int a[5][5] = {{0},{0,0,2,1,2},{0,1,0,1,2},{0,3,3,1,0},{0,1,3,2,3}};
  int p[5] = {0, 3, 1, 5, 8};
  for (int i = 1; i <= n; i++) { for (int j = 1; j <= m; j++) attr[i][j] = a[i][j]; price[i] = p[i]; }
  auto b = brute(n, m, attr, price);
  printf("暴力=(%d,%d)\n", b.first, b.second);
  return 0;
}
