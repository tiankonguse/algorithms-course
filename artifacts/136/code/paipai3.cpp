// 题3 对拍：课程魔力降序贪心 vs 暴力枚举线性无关子集最大魔力
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

bool indep(ll* states, int cnt) {
  // cnt 个元素线性无关 ⇔ 无非空子集异或和为 0
  ll basis[61] = {0};
  for (int i = 0; i < cnt; i++) {
    ll num = states[i];
    for (int b = 60; b >= 0; b--) {
      if (num >> b == 1) {
        if (basis[b] == 0) { basis[b] = num; break; }
        num ^= basis[b];
      }
    }
    if (num == 0) return false;  // 插入失败 = 相关
  }
  return true;
}

ll brute(ll* st, ll* val, int n) {
  ll best = 0;
  for (int m = 1; m < (1 << n); m++) {
    vector<ll> s; ll sum = 0;
    for (int i = 0; i < n; i++) if (m >> i & 1) { s.push_back(st[i]); sum += val[i]; }
    if (indep(s.data(), s.size())) best = max(best, sum);
  }
  return best;
}

int main() {
  mt19937_64 rng(13579);
  int T = 2000, bad = 0;
  for (int t = 0; t < T; t++) {
    int n = 1 + rng() % 7;
    ll st[10], val[10];
    for (int i = 0; i < n; i++) { st[i] = 1 + rng() % ((1LL << 60) - 1); val[i] = 1 + rng() % 10000; }
    // 课程
    vector<pair<ll,ll>> v;
    for (int i = 0; i < n; i++) v.push_back({st[i], val[i]});
    sort(v.begin(), v.end(), [](const pair<ll,ll>& a, const pair<ll,ll>& b) { return a.second > b.second; });
    ll basis[61] = {0};
    auto insert = [&](ll num) {
      for (int i = 60; i >= 0; i--) {
        if (num >> i == 1) {
          if (basis[i] == 0) { basis[i] = num; return true; }
          num ^= basis[i];
        }
      }
      return false;
    };
    ll cur = 0;
    for (int i = 0; i < n; i++) if (insert(v[i].first)) cur += v[i].second;
    ll ref = brute(st, val, n);
    if (cur != ref) {
      bad++;
      if (bad <= 3) {
        cout << "BAD n=" << n << " cur=" << cur << " ref=" << ref << endl;
        for (int i = 0; i < n; i++) cout << "  st=" << st[i] << " val=" << val[i] << endl;
      }
    }
  }
  cout << (bad ? "FAIL" : "ALL OK") << " bad=" << bad << " T=" << T << endl;
  return 0;
}
