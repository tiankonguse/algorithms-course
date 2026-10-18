#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MOD = 1000000007;

// ===== 课程版：Code01_GaussEor 改写 =====
const int MAXV = 2000;
int prime[MAXV+1], cnt;
bool vis[MAXV+1];
int pow2v[305];
int mat[305][305];   // [cnt][n+1]
int n;

void prepare() {
  for (int i = 2; i * i <= MAXV; i++) {
    if (!vis[i]) {
      for (int j = i * i; j <= MAXV; j += i) vis[j] = true;
    }
  }
  cnt = 0;
  for (int i = 2; i <= MAXV; i++) if (!vis[i]) prime[++cnt] = i;
  pow2v[0] = 1;
  for (int i = 1; i < 305; i++) pow2v[i] = (pow2v[i-1] * 2) % MOD;
}

void gauss(int gn) {
  for (int i = 1; i <= gn; i++) {
    for (int j = 1; j <= gn; j++) {
      if (j < i && mat[j][j] == 1) continue;
      if (mat[j][i] == 1) {
        for (int k = 1; k <= gn+1; k++) swap(mat[i][k], mat[j][k]);
        break;
      }
    }
    if (mat[i][i] == 1) {
      for (int j = 1; j <= gn; j++) {
        if (i != j && mat[j][i] == 1) {
          for (int k = i; k <= gn+1; k++) mat[j][k] ^= mat[i][k];
        }
      }
    }
  }
}

int course_solve(ll arr[]) {
  for (int i = 1; i <= cnt; i++)
    for (int j = 1; j <= cnt+1; j++) mat[i][j] = 0;
  ll cur;
  for (int i = 1; i <= n; i++) {
    cur = arr[i];
    for (int j = 1; j <= cnt && cur != 0; j++) {
      while (cur % prime[j] == 0) { mat[j][i] ^= 1; cur /= prime[j]; }
    }
  }
  gauss(cnt);
  int mainc = 0;
  for (int i = 1; i <= cnt; i++) if (mat[i][i] == 1) mainc++;
  return (pow2v[n - mainc] - 1 + MOD) % MOD;
}

// ===== 暴力版：枚举非空子集，质因子指数向量全偶 =====
int brute_solve(ll arr[]) {
  // 每项分解成指数向量 exp[i][p]
  static int expv[305][303];
  for (int i = 1; i <= n; i++) {
    ll cur = arr[i];
    for (int j = 1; j <= cnt && cur != 0; j++) {
      expv[i][j] = 0;
      while (cur % prime[j] == 0) { expv[i][j]++; cur /= prime[j]; }
    }
  }
  int ans = 0;
  for (int mask = 1; mask < (1 << n); mask++) {
    int ok = 1;
    for (int j = 1; j <= cnt; j++) {
      int s = 0;
      for (int i = 1; i <= n; i++) if (mask >> (i-1) & 1) s += expv[i][j];
      if (s & 1) { ok = 0; break; }
    }
    if (ok) ans++;
  }
  return ans;
}

// ===== 随机数据生成：质因子全 ≤ 2000 =====
mt19937 rng(20261016);
ll gen_arr() {
  // 选 1~4 个质数乘起来（指数 1~2），保证 ≤ 1e18
  while (true) {
    int k = 1 + rng() % 4;
    ll v = 1;
    for (int t = 0; t < k; t++) {
      int p = prime[1 + rng() % cnt];
      int e = 1 + rng() % 2;
      for (int x = 0; x < e; x++) {
        if (v > (ll)4e18 / p) { v = -1; break; }
        v *= p;
      }
      if (v < 0) break;
    }
    if (v > 0 && v <= (ll)1e18) return v;
  }
}

int main() {
  prepare();
  ll arr[305];
  for (int T = 0; T < 5000; T++) {
    n = 1 + rng() % 10;
    for (int i = 1; i <= n; i++) arr[i] = gen_arr();
    int c = course_solve(arr);
    int b = brute_solve(arr);
    if (c != b) {
      printf("MISMATCH T=%d n=%d course=%d brute=%d\n", T, n, c, b);
      for (int i = 1; i <= n; i++) printf("%lld ", arr[i]);
      printf("\n");
      return 1;
    }
  }
  printf("t1 OK 5000 groups\n");
  // 固定例：n=3, [4,9,36] -> 子集 {4*9=36, 4*36=144, 9*36=324, 4*9*36=1296} 4 个? 4=2^2,9=3^2,36=2^2*3^2
  // 指数向量: 4=(2,0) 9=(0,2) 36=(2,2). 非空子集和全偶: {4,9}: (2,2)✓ {36}: (2,2)✓ {4,9,36}:(4,4)✓ = 3 个
  n = 3; arr[1]=4; arr[2]=9; arr[3]=36;
  printf("fix1 [4,9,36] course=%d brute=%d (期望 3)\n", course_solve(arr), brute_solve(arr));
  // n=1, arr=[4] -> 子集 {4} 完全平方 ✓ -> 1
  n = 1; arr[1]=4;
  printf("fix2 [4] course=%d brute=%d (期望 1)\n", course_solve(arr), brute_solve(arr));
  // n=1, arr=[2] -> 非空子集 {2} 不是平方 -> 0
  n = 1; arr[1]=2;
  printf("fix3 [2] course=%d brute=%d (期望 0)\n", course_solve(arr), brute_solve(arr));
  return 0;
}
