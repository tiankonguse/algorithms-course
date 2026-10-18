// 题2 loj 114 第k小异或和：高斯消元标准基
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 100001;
const int BIT = 50;
long long arr[MAXN];
int len, n;
bool zero;

void compute() {
  len = 1;
  for (long long i = BIT; i >= 0; i--) {
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
}

long long query(long long k) {
  if (zero && k == 1) return 0;
  if (zero) k--;
  if (k >= 1LL << len) return -1;
  long long ans = 0;
  for (int i = len, j = 0; i >= 1; i--, j++) {
    if ((k & (1LL << j)) != 0) ans ^= arr[i];
  }
  return ans;
}

int main() {
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> arr[i];
  compute();
  int q;
  cin >> q;
  for (int i = 1; i <= q; i++) {
    long long k;
    cin >> k;
    cout << query(k) << endl;
  }
  return 0;
}
