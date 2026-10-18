#pragma once
#include <bits/stdc++.h>
using namespace std;

static long long mygcd(long long a, long long b) {
  if (a < 0) a = -a;
  if (b < 0) b = -b;
  return b == 0 ? a : mygcd(b, a % b);
}

// 有理数精确运算，作对拍的「参考答案」
struct Frac {
  long long p, q;
  Frac(long long p_ = 0, long long q_ = 1) : p(p_), q(q_) { norm(); }
  void norm() {
    if (q < 0) { p = -p; q = -q; }
    if (p == 0) { q = 1; return; }
    long long g = mygcd(p, q);
    p /= g; q /= g;
  }
  Frac operator+(const Frac& o) const { return Frac(p * o.q + o.p * q, q * o.q); }
  Frac operator*(const Frac& o) const { return Frac(p * o.p, q * o.q); }
  Frac operator/(const Frac& o) const { return Frac(p * o.q, q * o.p); }
  bool isZero() const { return p == 0; }
  long double toD() const { return (long double)p / q; }
};
Frac operator-(const Frac& a, const Frac& b) { return a + Frac(-b.p, b.q); }

// 高斯-约当全消元（有理数精确）。返回 -1 矛盾 / 0 多解 / 1 唯一解
// A[1..n][1..n+1]，解写入 sol[1..n]
int gauss_jordan(vector<vector<Frac>>& A, int n, vector<Frac>& sol) {
  vector<int> piv(n + 1, 0);
  int rank = 0;
  for (int col = 1; col <= n; col++) {
    int mx = -1;
    for (int r = rank + 1; r <= n; r++) if (!A[r][col].isZero()) { mx = r; break; }
    if (mx == -1) continue;
    swap(A[rank + 1], A[mx]);
    Frac pv = A[rank + 1][col];
    for (int c = col; c <= n + 1; c++) A[rank + 1][c] = A[rank + 1][c] / pv;
    for (int r = 1; r <= n; r++) {
      if (r != rank + 1 && !A[r][col].isZero()) {
        Frac rate = A[r][col];
        for (int c = col; c <= n + 1; c++) A[r][c] = A[r][c] - rate * A[rank + 1][c];
      }
    }
    piv[rank + 1] = col;
    rank++;
  }
  for (int r = rank + 1; r <= n; r++) {
    bool allZero = true;
    for (int c = 1; c <= n; c++) if (!A[r][c].isZero()) { allZero = false; break; }
    if (allZero && !A[r][n + 1].isZero()) return -1;
  }
  if (rank < n) return 0;
  sol.assign(n + 1, Frac(0));
  for (int r = 1; r <= n; r++) sol[piv[r]] = A[r][n + 1];
  return 1;
}
