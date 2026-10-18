// 题3 P4570 元素：魔力降序贪心，insert 成功累加
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 1001;
const int BIT = 60;
struct Ore { ll state, val; } arr[MAXN];
ll basis[BIT + 1];
int n;

bool insert(ll num) {
  for (int i = BIT; i >= 0; i--) {
    if (num >> i == 1) {
      if (basis[i] == 0) { basis[i] = num; return true; }
      num ^= basis[i];
    }
  }
  return false;
}

ll compute() {
  ll ans = 0;
  sort(arr + 1, arr + n + 1, [](const Ore& a, const Ore& b) { return a.val > b.val; });
  for (int i = 1; i <= n; i++) {
    if (insert(arr[i].state)) ans += arr[i].val;
  }
  return ans;
}

int main() {
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> arr[i].state >> arr[i].val;
  cout << compute() << endl;
  return 0;
}
