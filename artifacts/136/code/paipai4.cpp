// 题4 对拍：课程线性基大小 2^size vs 暴力枚举开关组合去重计数
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MOD = 2008;

int main() {
  mt19937_64 rng(86420);
  int T = 3000, bad = 0;
  for (int t = 0; t < T; t++) {
    int nn = 1 + rng() % 8;   // 灯泡数
    int m = 1 + rng() % 7;    // 开关数
    // 生成开关串
    vector<ll> sw(m);
    for (int i = 0; i < m; i++) {
      ll num = 0;
      for (int j = 0; j < nn; j++) if (rng() & 1) num |= 1LL << j;
      sw[i] = num;
    }
    // 课程
    ll basis[9] = {0};
    auto insert = [&](ll num) {
      for (int i = nn - 1; i >= 0; i--) {
        if (num >> i == 1) {
          if (basis[i] == 0) { basis[i] = num; return true; }
          num ^= basis[i];
        }
      }
      return false;
    };
    int size = 0;
    for (int i = 0; i < m; i++) if (insert(sw[i])) size++;
    ll cur = 1;
    for (int i = 0; i < size; i++) cur = (cur * 2) % MOD;
    // 暴力
    set<ll> states;
    for (int mask = 0; mask < (1 << m); mask++) {
      ll x = 0;
      for (int i = 0; i < m; i++) if (mask >> i & 1) x ^= sw[i];
      states.insert(x);
    }
    ll ref = states.size() % MOD;
    if (cur != ref) {
      bad++;
      if (bad <= 3) {
        cout << "BAD nn=" << nn << " m=" << m << " size=" << size << " cur=" << cur << " ref=" << ref << " states=" << states.size() << endl;
      }
    }
  }
  cout << (bad ? "FAIL" : "ALL OK") << " bad=" << bad << " T=" << T << endl;
  return 0;
}
