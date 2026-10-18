// 题1 对拍：课程 insert+贪心 vs 暴力全枚举
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll brute(ll* a, int n) {
  ll best = 0;
  for (int m = 1; m < (1 << n); m++) {
    ll x = 0;
    for (int i = 0; i < n; i++) if (m >> i & 1) x ^= a[i];
    best = max(best, x);
  }
  return best;
}

int main() {
  mt19937_64 rng(12345);
  int T = 3000, bad = 0;
  for (int t = 0; t < T; t++) {
    int n = 1 + rng() % 8;
    ll a[10];
    for (int i = 0; i < n; i++) a[i] = rng() & ((1LL << 50) - 1);
    // 课程
    ll basis[51] = {0};
    auto insert = [&](ll num) {
      for (int i = 50; i >= 0; i--) {
        if (num >> i == 1) {
          if (basis[i] == 0) { basis[i] = num; return true; }
          num ^= basis[i];
        }
      }
      return false;
    };
    for (int i = 0; i < n; i++) insert(a[i]);
    ll cur = 0;
    for (int i = 50; i >= 0; i--) cur = max(cur, cur ^ basis[i]);
    ll ref = brute(a, n);
    if (cur != ref) { bad++; if (bad <= 3) { cout << "BAD n=" << n << " cur=" << cur << " ref=" << ref << " a:"; for (int i = 0; i < n; i++) cout << " " << a[i]; cout << endl; } }
  }
  cout << (bad ? "FAIL" : "ALL OK") << " bad=" << bad << " T=" << T << endl;
  return 0;
}
