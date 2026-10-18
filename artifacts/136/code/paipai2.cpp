// 题2 对拍：课程高斯消元 query vs 暴力枚举去重排序
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<ll> bruteAll(ll* a, int n) {
  set<ll> s;
  for (int m = 1; m < (1 << n); m++) {
    ll x = 0;
    for (int i = 0; i < n; i++) if (m >> i & 1) x ^= a[i];
    s.insert(x);
  }
  return vector<ll>(s.begin(), s.end());
}

int main() {
  mt19937_64 rng(24680);
  int T = 3000, bad = 0;
  for (int t = 0; t < T; t++) {
    int n = 1 + rng() % 7;
    ll a[10];
    for (int i = 0; i < n; i++) a[i] = rng() & ((1LL << 50) - 1);
    // 课程
    ll arr[12] = {0};
    for (int i = 1; i <= n; i++) arr[i] = a[i - 1];
    int len = 1; bool zero = false;
    for (ll i = 50; i >= 0; i--) {
      for (int j = len; j <= n; j++) {
        if ((arr[j] & (1LL << i)) != 0) { swap(arr[j], arr[len]); break; }
      }
      if ((arr[len] & (1LL << i)) != 0) {
        for (int j = 1; j <= n; j++) {
          if (j != len && (arr[j] & (1LL << i)) != 0) arr[j] ^= arr[len];
        }
        len++;
      }
    }
    len--;
    zero = len != n;
    auto query = [&](ll k) {
      if (zero && k == 1) return 0LL;
      if (zero) k--;
      if (k >= 1LL << len) return -1LL;
      ll ans = 0;
      for (int i = len, j = 0; i >= 1; i--, j++) {
        if ((k & (1LL << j)) != 0) ans ^= arr[i];
      }
      return ans;
    };
    // 暴力
    vector<ll> all = bruteAll(a, n);
    int q = 1 + rng() % 8;
    for (int qi = 0; qi < q; qi++) {
      ll k = 1 + rng() % ((ll)all.size() + 3);
      ll cur = query(k);
      ll ref = (k <= (ll)all.size()) ? all[k - 1] : -1;
      if (cur != ref) {
        bad++;
        if (bad <= 3) {
          cout << "BAD n=" << n << " k=" << k << " cur=" << cur << " ref=" << ref << " zero=" << zero << " len=" << len << " a:";
          for (int i = 0; i < n; i++) cout << " " << a[i];
          cout << " all:";
          for (ll v : all) cout << " " << v;
          cout << endl;
        }
      }
    }
  }
  cout << (bad ? "FAIL" : "ALL OK") << " bad=" << bad << " T=" << T << endl;
  return 0;
}
