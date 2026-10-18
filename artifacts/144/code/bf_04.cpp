#include <bits/stdc++.h>
using namespace std;
// 暴力（课上版）：照 Code04_SplitWays1.ways1，B[i],C[i] >= 1
int fv(const vector<int>& arr, int i, int preb, int prec) {
  if (i == (int)arr.size()) return 1;
  int ans = 0;
  for (int b = 1, c = arr[i]-1; b < arr[i]; b++, c--)
    if (preb <= b && prec >= c) ans += fv(arr, i+1, b, c);
  return ans;
}
int ways1(const vector<int>& arr) {
  int ans = 0;
  for (int b = 1, c = arr[0]-1; b < arr[0]; b++, c--) ans += fv(arr, 1, b, c);
  return ans;
}
// 暴力（LeetCode 版）：B[i] 从 0..arr[i]（C[i] = arr[i]-B[i] >= 0）
int fv0(const vector<int>& arr, int i, int preb, int prec) {
  if (i == (int)arr.size()) return 1;
  int ans = 0;
  for (int b = 0, c = arr[i]; b <= arr[i]; b++, c--)
    if (preb <= b && prec >= c) ans += fv0(arr, i+1, b, c);
  return ans;
}
int ways0(const vector<int>& arr) {
  int ans = 0;
  for (int b = 0, c = arr[0]; b <= arr[0]; b++, c--) ans += fv0(arr, 1, b, c);
  return ans;
}
int main() {
  int n; cin >> n;
  vector<int> arr(n);
  for (int i = 0; i < n; i++) cin >> arr[i];
  cout << ways1(arr) << " " << ways0(arr) << "\n";
}
