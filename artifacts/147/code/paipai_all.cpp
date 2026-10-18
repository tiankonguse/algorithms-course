// 147 课卡特兰数 9 题对拍：课程实现 vs 暴力
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ===== 课程实现 =====
// 题1 P1044 卡特兰数模版 mod 1e9+7（四公式）
const int MOD1 = 1000000007;
ll fac1[1000001], inv1t[1000001], inv2t[1000001];
ll power1(ll x, ll p) { ll ans = 1; while (p) { if (p & 1) ans = ans * x % MOD1; x = x * x % MOD1; p >>= 1; } return ans; }
void build1a(int n) { fac1[0] = inv1t[0] = 1; fac1[1] = 1; for (int i = 2; i <= n; i++) fac1[i] = (ll)i * fac1[i-1] % MOD1; inv1t[n] = power1(fac1[n], MOD1-2); for (int i = n-1; i >= 1; i--) inv1t[i] = (ll)(i+1) * inv1t[i+1] % MOD1; }
void build1b(int n) { inv2t[1] = 1; for (int i = 2; i <= n+1; i++) inv2t[i] = MOD1 - inv2t[MOD1 % i] * (MOD1 / i) % MOD1; }
ll c1(int n, int k) { return (((fac1[n] * inv1t[k]) % MOD1) * inv1t[n-k]) % MOD1; }
ll catalan_formula1(int n) { build1a(2*n); return (c1(2*n, n) - c1(2*n, n-1) + MOD1) % MOD1; }
ll catalan_formula2(int n) { build1a(2*n); return c1(2*n, n) * power1(n+1, MOD1-2) % MOD1; }
ll catalan_formula3(int n) { build1b(n); ll f[1001] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) f[i] = f[i-1] * (4*i-2) % MOD1 * inv2t[i+1] % MOD1; return f[n]; }
ll catalan_formula4(int n) { ll f[1001] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] = (f[i] + f[l] * f[r] % MOD1) % MOD1; return f[n]; }

// 题2 P1976 圆上连线 mod 1e8+7 公式1
const int MOD2 = 100000007;
ll fac2[2000001], inv2t2[2000001];
ll power2(ll x, ll p) { ll ans = 1; while (p) { if (p & 1) ans = ans * x % MOD2; x = x * x % MOD2; p >>= 1; } return ans; }
void build2(int n) { fac2[0] = inv2t2[0] = 1; fac2[1] = 1; for (int i = 2; i <= n; i++) fac2[i] = (ll)i * fac2[i-1] % MOD2; inv2t2[n] = power2(fac2[n], MOD2-2); for (int i = n-1; i >= 1; i--) inv2t2[i] = (ll)(i+1) * inv2t2[i+1] % MOD2; }
ll c2(int n, int k) { return (((fac2[n] * inv2t2[k]) % MOD2) * inv2t2[n-k]) % MOD2; }
ll circle_compute(int n) { build2(2*n); return (c2(2*n, n) - c2(2*n, n-1) + MOD2) % MOD2; }

// 题3 P1722 红黑前缀 mod 100 公式4
ll redmore_compute(int n) { ll f[101] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] = (f[i] + f[l] * f[r] % 100) % 100; return f[n]; }

// 题4 LeetCode 96 二叉树数量 公式4 int
int numTrees(int n) { int f[20] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] += f[l] * f[r]; return f[n]; }

// 题5 P3200 有趣的数列 因子计数法 mod p
int minpf[2000001], prime[2000001], cnt, counts5[2000001];
void euler(int n) { memset(minpf, 0, sizeof(minpf)); cnt = 0; for (int i = 2; i <= n; i++) { if (minpf[i] == 0) prime[cnt++] = i; for (int j = 0; j < cnt; j++) { if ((ll)i * prime[j] > n) break; minpf[i * prime[j]] = prime[j]; if (i % prime[j] == 0) break; } } }
ll power5(ll x, ll p, int mod) { ll ans = 1; while (p) { if (p & 1) ans = ans * x % mod; x = x * x % mod; p >>= 1; } return ans; }
int funny_compute(int n, int mod) {
    euler(2 * n);
    for (int i = 2; i <= n; i++) counts5[i] = -1;
    for (int i = n + 2; i <= 2 * n; i++) counts5[i] = 1;
    counts5[n + 1] = 0;
    for (int i = 2 * n; i >= 2; i--) {
        if (minpf[i] != 0) {
            counts5[minpf[i]] += counts5[i];
            counts5[i / minpf[i]] += counts5[i];
            counts5[i] = 0;
        }
    }
    ll ans = 1;
    for (int i = 2; i <= 2 * n; i++) if (counts5[i] != 0) ans = ans * power5(i, counts5[i], mod) % mod;
    return (int)ans;
}

// 题6 P1641 生成字符串 C(n+m,m)-C(n+m,m-1) mod 20100403
const int MOD6 = 20100403;
ll fac6[2000001], inv6[2000001];
ll power6(ll x, ll p) { ll ans = 1; while (p) { if (p & 1) ans = ans * x % MOD6; x = x * x % MOD6; p >>= 1; } return ans; }
void build6(int n) { fac6[0] = inv6[0] = 1; fac6[1] = 1; for (int i = 2; i <= n; i++) fac6[i] = (ll)i * fac6[i-1] % MOD6; inv6[n] = power6(fac6[n], MOD6-2); for (int i = n-1; i >= 1; i--) inv6[i] = (ll)(i+1) * inv6[i+1] % MOD6; }
ll c6(int n, int k) { return (((fac6[n] * inv6[k]) % MOD6) * inv6[n-k]) % MOD6; }
ll genstr_compute(int n, int m) { build6(n + m); return (c6(n + m, m) - c6(n + m, m - 1) + MOD6) % MOD6; }

// 题7 SP7897 不含递增三元组 mod 1e6 公式4
ll f7[1001];
void build7() { f7[0] = f7[1] = 1; for (int i = 2; i < 1001; i++) for (int l = 0, r = i-1; l < i; l++, r--) f7[i] = (f7[i] + f7[l] * f7[r] % 1000000) % 1000000; }

// 题8 P2532 树屋阶梯 因子计数法大数（等价公式2，避免大数除法）
struct Big { vector<int> d; // 低位在前
    Big(int v = 1) { if (v == 0) { d.push_back(0); return; } while (v) { d.push_back(v % 10); v /= 10; } }
    void mul(int x) { ll carry = 0; for (int i = 0; i < (int)d.size(); i++) { ll cur = (ll)d[i] * x + carry; d[i] = cur % 10; carry = cur / 10; } while (carry) { d.push_back(carry % 10); carry /= 10; } }
    string str() { string s; for (int i = (int)d.size() - 1; i >= 0; i--) s += char('0' + d[i]); return s; }
};
string ladder_compute(int n) {
    euler(2 * n);
    for (int i = 2; i <= n; i++) counts5[i] = -1;
    for (int i = n + 2; i <= 2 * n; i++) counts5[i] = 1;
    counts5[n + 1] = 0;
    for (int i = 2 * n; i >= 2; i--) {
        if (minpf[i] != 0) {
            counts5[minpf[i]] += counts5[i];
            counts5[i / minpf[i]] += counts5[i];
            counts5[i] = 0;
        }
    }
    Big ans(1);
    for (int i = 2; i <= 2 * n; i++) {
        int e = counts5[i];
        for (int j = 0; j < e; j++) ans.mul(i);
    }
    return ans.str();
}

// 题9 P3978 叶子期望
double leaf_expectation(ll n) { return (double)n * (n + 1) / ((2 * n - 1) * 2.0); }

// ===== 暴力 =====
// 题1 进出栈 DFS
ll brute_stack(int n) { ll cnt = 0; function<void(int,int)> dfs = [&](int a, int b) { if (a == n && b == n) { cnt++; return; } if (a < n) dfs(a + 1, b); if (b < a) dfs(a, b + 1); }; dfs(0, 0); return cnt; }
// 题2 公式4 精确（无除法）
ll brute_circle(int n) { ll f[21] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] += f[l] * f[r]; return f[n]; }
// 题3 红黑 DFS
ll brute_redmore(int n) { ll cnt = 0; function<void(int,int)> dfs = [&](int r, int b) { if (r == n && b == n) { cnt++; return; } if (r < n) dfs(r + 1, b); if (b < n && b < r) dfs(r, b + 1); }; dfs(0, 0); return cnt; }
// 题4 公式4 精确 long long
ll brute_trees(int n) { ll f[21] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] += f[l] * f[r]; return f[n]; }
// 题5 公式4 mod p
ll brute_funny(int n, int mod) { ll f[31] = {}; f[0] = f[1] = 1; for (int i = 2; i <= n; i++) for (int l = 0, r = i-1; l < i; l++, r--) f[i] = (f[i] + f[l] * f[r] % mod) % mod; return f[n]; }
// 题6 生成字符串 DFS
ll brute_genstr(int n, int m) { ll cnt = 0; function<void(int,int)> dfs = [&](int o, int z) { if (o == n && z == m) { cnt++; return; } if (o < n) dfs(o + 1, z); if (z < m && z < o) dfs(o, z + 1); }; dfs(0, 0); return cnt; }
// 题7 无递增三元组排列
ll brute_skyline(int n) { vector<int> a(n); for (int i = 0; i < n; i++) a[i] = i + 1; ll cnt = 0; do { bool ok = true; for (int i = 0; i < n && ok; i++) for (int j = i + 1; j < n && ok; j++) for (int k = j + 1; k < n; k++) if (a[i] < a[j] && a[j] < a[k]) { ok = false; break; } if (ok) cnt++; } while (next_permutation(a.begin(), a.end())); return cnt; }
// 题9 枚举二叉树结构统计叶子
pair<ll,ll> brute_leaf(int n) { // {结构数, 叶子总数}
    static map<int,pair<ll,ll>> memo;
    if (memo.count(n)) return memo[n];
    if (n == 0) return memo[n] = {1, 0};
    if (n == 1) return memo[n] = {1, 1};
    pair<ll,ll> res = {0, 0};
    for (int i = 0; i <= n - 1; i++) {
        auto L = brute_leaf(i), R = brute_leaf(n - 1 - i);
        ll ways = L.first * R.first;
        res.first += ways;
        res.second += L.second * R.first + R.second * L.first;
    }
    return memo[n] = res;
}

// ===== 随机 =====
mt19937 rng(20261017);
int rnd(int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(rng); }

int main() {
    int fails = 0, total = 0;
    // 题1
    for (int it = 0; it < 200; it++) { int n = rnd(1, 12); total++; ll want = brute_stack(n) % MOD1; ll g1 = catalan_formula1(n), g2 = catalan_formula2(n), g3 = catalan_formula3(n), g4 = catalan_formula4(n); if (g1 != want || g2 != want || g3 != want || g4 != want) { fails++; printf("T1 FAIL n=%d want=%lld f1=%lld f2=%lld f3=%lld f4=%lld\n", n, want, g1, g2, g3, g4); } }
    // 题2
    for (int it = 0; it < 300; it++) { int n = rnd(1, 10); total++; if (circle_compute(n) != brute_circle(n) % MOD2) { fails++; printf("T2 FAIL n=%d\n", n); } }
    // 题3
    for (int it = 0; it < 300; it++) { int n = rnd(1, 12); total++; if (redmore_compute(n) != brute_redmore(n) % 100) { fails++; printf("T3 FAIL n=%d\n", n); } }
    // 题4
    for (int it = 0; it < 300; it++) { int n = rnd(1, 19); total++; if (numTrees(n) != brute_trees(n)) { fails++; printf("T4 FAIL n=%d\n", n); } }
    // 题5
    for (int it = 0; it < 1000; it++) { int n = rnd(1, 30); int p = rnd(1, 1000000000); total++; if (funny_compute(n, p) != brute_funny(n, p)) { fails++; printf("T5 FAIL n=%d p=%d\n", n, p); } }
    // 题6
    for (int it = 0; it < 300; it++) { int n = rnd(1, 10), m = rnd(1, n); total++; if (genstr_compute(n, m) != brute_genstr(n, m) % MOD6) { fails++; printf("T6 FAIL n=%d m=%d\n", n, m); } }
    // 题7
    build7();
    for (int n = 1; n <= 9; n++) { total++; if (f7[n] != brute_skyline(n) % 1000000) { fails++; printf("T7 FAIL n=%d\n", n); } }
    // 题8 单独脚本对比（Python 大整数），这里只输出抽查值
    for (int n : {1, 2, 3, 10, 30, 100, 300, 500}) { printf("T8 n=%d = %s\n", n, ladder_compute(n).c_str()); }
    // 题9
    for (int it = 0; it < 50; it++) { int n = rnd(1, 10); total++; auto b = brute_leaf(n); double want = (double)b.second / b.first; double got = leaf_expectation(n); if (fabs(got - want) > 1e-9) { fails++; printf("T9 FAIL n=%d want=%.9f got=%.9f\n", n, want, got); } }
    printf("== total %d cases, fails %d ==\n", total, fails);
    // 题9 已知值抽查
    for (int n : {1, 2, 3, 100, 1000000000}) printf("T9 n=%d = %.9f\n", n, leaf_expectation(n));
    return 0;
}
