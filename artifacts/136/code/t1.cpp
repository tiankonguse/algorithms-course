// 题1 P3812 最大异或和：普通消元建线性基，从高位贪心
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 51;
const int BIT = 50;
long long arr[MAXN];
long long basis[BIT + 1];
int n;

bool insert(long long num) {
  for (int i = BIT; i >= 0; i--) {
    if (num >> i == 1) {
      if (basis[i] == 0) {
        basis[i] = num;
        return true;
      }
      num ^= basis[i];
    }
  }
  return false;
}

long long compute() {
  for (int i = 1; i <= n; i++) insert(arr[i]);
  long long ans = 0;
  for (int i = BIT; i >= 0; i--) {
    ans = max(ans, ans ^ basis[i]);
  }
  return ans;
}

int main() {
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> arr[i];
  cout << compute() << endl;
  return 0;
}
