#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
// LeetCode 版（Code04_SplitWays2）：分裂 v+1 种，k = arr[0] + 1
long long power(long long x, long long p) { long long ans=1; while(p>0){ if(p&1) ans=ans*x%MOD; x=x*x%MOD; p>>=1;} return ans; }
long long comb(int n, int k) {
  long long fac=1, inv1=1, inv2=1;
  for (int i=1;i<=n;i++) {
    fac = fac*i%MOD;
    if (i==k) inv1 = power(fac, MOD-2);
    if (i==n-k) inv2 = power(fac, MOD-2);
  }
  return fac*inv1%MOD*inv2%MOD;
}
int main() {
  int n; cin >> n;
  vector<int> arr(n);
  for (int i=0;i<n;i++) cin >> arr[i];
  int k = arr[0]+1;
  for (int i=1;i<n && k>0;i++) if (arr[i-1] > arr[i]) k -= arr[i-1]-arr[i];
  if (k <= 0) { cout << 0 << "\n"; return 0; }
  cout << comb(k+n-1, n) << "\n";
}
