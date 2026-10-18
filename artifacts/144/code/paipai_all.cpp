#include <bits/stdc++.h>
using namespace std;
// 144 课对拍：课程实现 vs 暴力，全内嵌单程序，随机小用例。
mt19937 rng(20261017);
int rnd(int l, int r) { return (int)(rng() % (r - l + 1)) + l; }

// ===== 题2：计算系数 =====
const int MOD2 = 10007;
long long power2(long long x, int p, int mod) { long long ans=1; while(p>0){ if(p&1) ans=ans*x%mod; x=x*x%mod; p>>=1;} return ans; }
long long c2(int n, int k) {
  static long long fac[1002], inv[1002];
  static bool built = false;
  if (!built) {
    fac[0] = 1; fac[1] = 1;
    for (int i = 2; i <= 1001; i++) fac[i] = (long long)i * fac[i-1] % MOD2;
    inv[1001] = power2(fac[1001], MOD2-2, MOD2);
    for (int i = 1000; i >= 0; i--) inv[i] = (long long)(i+1) * inv[i+1] % MOD2;
    built = true;
  }
  return fac[n] * inv[k] % MOD2 * inv[n-k] % MOD2;
}
int course02(int a, int b, int k, int n, int m) {
  return (int)(power2(a, n, MOD2) * power2(b, m, MOD2) % MOD2 * c2(k, k-n) % MOD2);
}
int bf02(int a, int b, int k, int n, int m) {
  static long long coef[40][40], nx[40][40];
  memset(coef, 0, sizeof(coef));
  coef[0][0] = 1 % MOD2;
  for (int step = 0; step < k; step++) {
    memset(nx, 0, sizeof(nx));
    for (int i = 0; i <= step; i++)
      for (int j = 0; j <= step; j++) {
        if (!coef[i][j]) continue;
        nx[i+1][j] = (nx[i+1][j] + coef[i][j] * (a % MOD2)) % MOD2;
        nx[i][j+1] = (nx[i][j+1] + coef[i][j] * (b % MOD2)) % MOD2;
      }
    memcpy(coef, nx, sizeof(coef));
  }
  return (int)(coef[n][m] % MOD2);
}

// ===== 题3：组合数问题 =====
const int MAXV3 = 2000;
static int cc3[2002][2002], f3[2002][2002], sum3[2002][2002];
void build3(int k) {
  static int builtk = -1;
  if (builtk == k) return;
  builtk = k;
  for (int i = 0; i <= MAXV3; i++) {
    cc3[i][0] = 1 % k;
    for (int j = 1; j <= i; j++) cc3[i][j] = (cc3[i-1][j] + cc3[i-1][j-1]) % k;
  }
  for (int i = 1; i <= MAXV3; i++)
    for (int j = 1; j <= i; j++) f3[i][j] = cc3[i][j] == 0 ? 1 : 0;
  for (int i = 2; i <= MAXV3; i++) {
    for (int j = 1; j <= i; j++)
      sum3[i][j] = sum3[i][j-1] + sum3[i-1][j] - sum3[i-1][j-1] + f3[i][j];
    sum3[i][i+1] = sum3[i][i];
  }
}
int course03(int k, int n, int m) { build3(k); return m > n ? sum3[n][n] : sum3[n][m]; }
int bf03(int k, int n, int m) {
  static int tc[41][41];
  for (int i = 0; i <= 40; i++) {
    tc[i][0] = 1 % k;
    for (int j = 1; j <= i; j++) tc[i][j] = (tc[i-1][j] + tc[i-1][j-1]) % k;
  }
  int ans = 0;
  for (int i = 0; i <= n; i++)
    for (int j = 0; j <= min(i, m); j++)
      if (tc[i][j] == 0) ans++;
  return ans;
}

// ===== 题4：分割方法数 =====
const int MOD4 = 1000000007;
long long power4(long long x, long long p) { long long ans=1; while(p>0){ if(p&1) ans=ans*x%MOD4; x=x*x%MOD4; p>>=1;} return ans; }
long long comb4(int n, int k) {
  long long fac=1, inv1=1, inv2=1;
  for (int i=1;i<=n;i++) {
    fac = fac*i%MOD4;
    if (i==k) inv1 = power4(fac, MOD4-2);
    if (i==n-k) inv2 = power4(fac, MOD4-2);
  }
  return fac*inv1%MOD4*inv2%MOD4;
}
int course04(const vector<int>& arr) { // 课上版 k = arr[0]-1
  int n = (int)arr.size();
  int k = arr[0]-1;
  for (int i=1;i<n && k>0;i++) if (arr[i-1] > arr[i]) k -= arr[i-1]-arr[i];
  if (k <= 0) return 0;
  return (int)comb4(k+n-1, n);
}
int course05(const vector<int>& arr) { // LC 版 k = arr[0]+1
  int n = (int)arr.size();
  int k = arr[0]+1;
  for (int i=1;i<n && k>0;i++) if (arr[i-1] > arr[i]) k -= arr[i-1]-arr[i];
  if (k <= 0) return 0;
  return (int)comb4(k+n-1, n);
}
int fv1(const vector<int>& arr, int i, int preb, int prec) {
  if (i == (int)arr.size()) return 1;
  int ans = 0;
  for (int b = 1, c = arr[i]-1; b < arr[i]; b++, c--)
    if (preb <= b && prec >= c) ans += fv1(arr, i+1, b, c);
  return ans;
}
int ways1(const vector<int>& arr) {
  int ans = 0;
  for (int b = 1, c = arr[0]-1; b < arr[0]; b++, c--) ans += fv1(arr, 1, b, c);
  return ans;
}
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
  int fails = 0;
  // 题2
  {
    int ok = 0, bad = 0;
    for (int it = 0; it < 30000; it++) {
      int k = rnd(0, 8), a = rnd(0, 10), b = rnd(0, 10);
      int n = rnd(0, k), m = k - n;
      int c = course02(a, b, k, n, m), v = bf02(a, b, k, n, m);
      if (c == v) ok++; else { bad++; if (bad <= 5) printf("MISMATCH 题2 %d %d %d %d %d course=%d bf=%d\n", a,b,k,n,m,c,v); }
    }
    printf("题2 计算系数: OK=%d MISMATCH=%d\n", ok, bad);
    if (bad) fails++;
  }
  // 题3
  {
    int ok = 0, bad = 0;
    for (int it = 0; it < 20000; it++) {
      int k = rnd(2, 8), n = rnd(0, 40), m = rnd(0, 40);
      int c = course03(k, n, m), v = bf03(k, n, m);
      if (c == v) ok++; else { bad++; if (bad <= 5) printf("MISMATCH 题3 k=%d n=%d m=%d course=%d bf=%d\n", k,n,m,c,v); }
    }
    printf("题3 组合数问题: OK=%d MISMATCH=%d\n", ok, bad);
    if (bad) fails++;
  }
  // 题4（课上版 + LC 版）
  {
    int ok = 0, bad = 0;
    for (int it = 0; it < 3000; it++) {
      int n = rnd(1, 6);
      vector<int> arr(n);
      for (int i = 0; i < n; i++) arr[i] = rnd(1, 9);
      int c4 = course04(arr), c5 = course05(arr);
      int v1 = ways1(arr), v0 = ways0(arr);
      if (c4 == v1 && c5 == v0) ok++;
      else {
        bad++;
        if (bad <= 5) {
          printf("MISMATCH 题4 arr=");
          for (int x : arr) printf("%d ", x);
          printf("course4=%d course5=%d bf1=%d bf0=%d\n", c4, c5, v1, v0);
        }
      }
    }
    printf("题4 分割方法数: OK=%d MISMATCH=%d\n", ok, bad);
    if (bad) fails++;
  }
  printf(fails ? "=== 有 MISMATCH ===\n" : "=== 全过 ===\n");
  return fails ? 1 : 0;
}
