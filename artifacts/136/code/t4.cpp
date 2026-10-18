// 题4 P3857 彩灯：线性基大小 size → 2^size mod 2008
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 51;
const int MOD = 2008;
ll arr[MAXN];
ll basis[MAXN];
int n, m;

bool insert(ll num) {
  for (int i = n; i >= 0; i--) {
    if (num >> i == 1) {
      if (basis[i] == 0) { basis[i] = num; return true; }
      num ^= basis[i];
    }
  }
  return false;
}

int compute() {
  int size = 0;
  for (int i = 1; i <= m; i++) {
    if (insert(arr[i])) size++;
  }
  return size;
}

int main() {
  cin >> n >> m;
  n = n - 1;  // n 个灯泡 → 最高位 n-1
  string s;
  for (int i = 1; i <= m; i++) {
    cin >> s;
    ll num = 0;
    for (int j = 0; j <= n; j++) {
      if (s[j] == 'O') num |= 1LL << j;
    }
    arr[i] = num;
  }
  int size = compute();
  ll ans = 1;
  for (int i = 0; i < size; i++) ans = (ans * 2) % MOD;
  cout << ans << endl;
  return 0;
}
