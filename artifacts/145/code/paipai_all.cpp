// 145 课 5 题对拍：课程实现（C++ 移植） vs 暴力，单程序内嵌
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int TOT_FAIL = 0;

// ========== 题1 错排 ==========
ll der_ways1(int n) { // DP
    vector<ll> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        if (i == 1) dp[i] = 0;
        else if (i == 2) dp[i] = 1;
        else dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2]);
    }
    return dp[n];
}
ll der_ways2(int n) { // 二项式反演：Σ (-1)^i n!/i!
    ll facn = 1;
    for (int i = 1; i <= n; i++) facn *= i;
    ll ans = facn, faci = 1;
    for (int i = 1; i <= n; i++) {
        faci *= i;
        if ((i & 1) == 0) ans += facn / faci;
        else ans -= facn / faci;
    }
    return ans;
}
ll der_brute(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    ll cnt = 0;
    do {
        bool ok = true;
        for (int i = 0; i < n; i++) if (p[i] == i) { ok = false; break; }
        if (ok) cnt++;
    } while (next_permutation(p.begin(), p.end()));
    return cnt;
}
void test1() {
    int bad = 0;
    for (int n = 1; n <= 8; n++) {
        ll a = der_ways1(n), b = der_ways2(n), c = der_brute(n);
        if (a != c || b != c) { bad++; printf("  n=%d ways1=%lld ways2=%lld brute=%lld\n", n, a, b, c); }
    }
    // 已知 D(20) = 895014631192902121
    if (der_ways2(20) != 895014631192902121LL) { bad++; printf("  n=20 ways2=%lld 期望 895014631192902121\n", der_ways2(20)); }
    printf("题1 错排: %s (%d)\n", bad ? "MISMATCH" : "OK", bad);
    if (bad) TOT_FAIL++;
}

// ========== 题2 集合计数 ==========
const int MOD2 = 1000000007;
ll power2(ll x, ll p, ll mod) {
    ll ans = 1;
    while (p > 0) { if (p & 1) ans = ans * x % mod; x = x * x % mod; p >>= 1; }
    return ans;
}
ll set_course(int n, int k) {
    // fac/inv
    vector<ll> fac(n + 1), inv(n + 1);
    fac[0] = inv[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = (ll)i * fac[i - 1] % MOD2;
    if (n > 0) {
        inv[n] = power2(fac[n], MOD2 - 2, MOD2);
        for (int i = n - 1; i >= 1; i--) inv[i] = (ll)(i + 1) * inv[i + 1] % MOD2;
    }
    auto C = [&](int nn, int kk) {
        if (kk < 0 || kk > nn) return 0LL;
        return fac[nn] * inv[kk] % MOD2 * inv[nn - kk] % MOD2;
    };
    vector<ll> g(n + 1);
    ll tmp = 2;
    for (int i = n; i >= 0; i--) { g[i] = tmp; tmp = tmp * tmp % MOD2; }
    for (int i = 0; i <= n; i++) g[i] = (g[i] + MOD2 - 1) * C(n, i) % MOD2;
    ll ans = 0;
    for (int i = k; i <= n; i++) {
        ll term = C(i, k) * g[i] % MOD2;
        if (((i - k) & 1) == 0) ans = (ans + term) % MOD2;
        else ans = (ans + term * (MOD2 - 1)) % MOD2;
    }
    return ans;
}
ll set_brute(int n, int k) {
    // 全集 {0..n-1}，2^n 个子集；非空挑法枚举（子集掩码集合）
    int m = 1 << n;            // 2^n 个子集
    ll cnt = 0;
    // 挑法掩码从 1 到 2^m - 1
    ll total = (1LL << m) - 1; // n=4 -> 65535 OK
    for (ll pick = 1; pick <= total; pick++) {
        int inter = -1;
        for (int s = 0; s < m; s++) {
            if (pick >> s & 1) {
                if (inter == -1) inter = s;
                else inter &= s;
            }
        }
        int pc = __builtin_popcount(inter);
        if (pc == k) cnt = (cnt + 1) % MOD2;
    }
    return cnt;
}
void test2() {
    int bad = 0, tot = 0;
    for (int n = 1; n <= 4; n++)
        for (int k = 0; k <= n; k++) {
            ll a = set_course(n, k), b = set_brute(n, k);
            tot++;
            if (a != b) { bad++; printf("  n=%d k=%d course=%lld brute=%lld\n", n, k, a, b); }
        }
    printf("题2 集合计数: %s (%d/%d)\n", bad ? "MISMATCH" : "OK", bad, tot);
    if (bad) TOT_FAIL++;
}

// ========== 题3 分特产 ==========
const int MOD3 = 1000000007;
ll dis_course(int n, int m, vector<int>& arr) {
    int k = n * 2;
    // 课程原版：c 表全尺寸 MAXK=2002，只 fill 到 k=2n 行；行号 > 2n 时值为 0（Java 默认）
    vector<vector<ll>> c(2002, vector<ll>(2002, 0));
    for (int i = 0; i <= k; i++) {
        c[i][0] = 1;
        for (int j = 1; j <= i; j++) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % MOD3;
    }
    vector<ll> g(n + 1);
    for (int i = 0; i < n; i++) {
        g[i] = c[n][i];
        for (int j = 1; j <= m; j++) g[i] = g[i] * c[arr[j] + n - i - 1][n - i - 1] % MOD3;
    }
    g[n] = 0;
    ll ans = 0;
    for (int i = 0; i <= n; i++) {
        if ((i & 1) == 0) ans = (ans + g[i]) % MOD3;
        else ans = (ans + g[i] * (MOD3 - 1)) % MOD3;
    }
    return ans;
}
// 暴力：按特产递归分配数量向量，最后检查每人 >= 1
ll dis_brute_rec(int j, int n, int m, vector<int>& arr, vector<int>& got, ll& cnt) {
    if (j > m) {
        for (int i = 0; i < n; i++) if (got[i] == 0) return cnt;
        cnt = (cnt + 1) % MOD3;
        return cnt;
    }
    // 第 j 种特产 arr[j] 个分给 n 个人
    int rem = arr[j];
    vector<int> v(n);
    function<void(int,int)> rec = [&](int idx, int left) {
        if (idx == n - 1) { v[idx] = left; for (int i = 0; i < n; i++) got[i] += v[i]; dis_brute_rec(j + 1, n, m, arr, got, cnt); for (int i = 0; i < n; i++) got[i] -= v[i]; return; }
        for (int t = 0; t <= left; t++) { v[idx] = t; rec(idx + 1, left - t); }
    };
    rec(0, rem);
    return cnt;
}
ll dis_brute(int n, int m, vector<int>& arr) {
    vector<int> got(n, 0);
    ll cnt = 0;
    dis_brute_rec(1, n, m, arr, got, cnt);
    return cnt;
}
void test3() {
    mt19937 rng(145);
    int bad = 0;
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 4 + 1;       // 1..4
        int m = rng() % 3 + 1;       // 1..3（课程代码对 m=0 有边界错值：g[n] 硬设 0，真实数据 m>=1）
        vector<int> arr(m + 1);
        for (int j = 1; j <= m; j++) arr[j] = rng() % (n + 1) + 1; // 1..n+1，保证行号 <= 2n
        ll a = dis_course(n, m, arr), b = dis_brute(n, m, arr);
        if (a != b) {
            bad++;
            if (bad <= 5) { printf("  n=%d m=%d arr=", n, m); for (int j = 1; j <= m; j++) printf("%d ", arr[j]); printf(" course=%lld brute=%lld\n", a, b); }
        }
    }
    printf("题3 分特产: %s (%d bad/3000)\n", bad ? "MISMATCH" : "OK", bad);
    if (bad) TOT_FAIL++;
}

// ========== 题4 糖果药片 ==========
const int MOD4 = 1000000009;
ll nf_course(int n, int k0, vector<int>& a, vector<int>& b) {
    int k = k0;
    if (((n + k) & 1) == 0) k = (n + k) / 2;
    else return 0;
    sort(a.begin() + 1, a.end());
    sort(b.begin() + 1, b.end());
    vector<int> small(n + 1);
    for (int i = 1, cnt = 0; i <= n; i++) {
        while (cnt + 1 <= n && b[cnt + 1] < a[i]) cnt++;
        small[i] = cnt;
    }
    vector<ll> fac(n + 1, 1);
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD4;
    vector<vector<ll>> c(n + 1, vector<ll>(n + 1, 0));
    for (int i = 0; i <= n; i++) { c[i][0] = 1; for (int j = 1; j <= i; j++) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % MOD4; }
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        dp[i][0] = dp[i - 1][0];
        for (int j = 1; j <= i; j++) {
            dp[i][j] = (dp[i - 1][j] + dp[i - 1][j - 1] * (small[i] - j + 1) % MOD4) % MOD4;
        }
    }
    vector<ll> g(n + 1);
    for (int i = 0; i <= n; i++) g[i] = fac[n - i] * dp[n][i] % MOD4;
    ll ans = 0;
    for (int i = k; i <= n; i++) {
        ll term = c[i][k] * g[i] % MOD4;
        if (((i - k) & 1) == 0) ans = (ans + term) % MOD4;
        else ans = (ans + term * (MOD4 - 1)) % MOD4;
    }
    return ans;
}
ll nf_brute(int n, int k0, vector<int>& a, vector<int>& b) {
    // 枚举 b 全排列配对
    int need = (n + k0) / 2;
    if (((n + k0) & 1) != 0) return 0;
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 1);
    ll cnt = 0;
    do {
        int big = 0;
        for (int i = 1; i <= n; i++) if (a[i] > b[perm[i - 1]]) big++;
        if (big == need) cnt = (cnt + 1) % MOD4;
    } while (next_permutation(perm.begin(), perm.end()));
    return cnt;
}
void test4() {
    mt19937 rng(145);
    int bad = 0;
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 6 + 1; // 1..6
        int k0 = rng() % (n + 1);
        // 值全不同：shuffle 1..(n*2)
        vector<int> vals(n * 2);
        iota(vals.begin(), vals.end(), 1);
        shuffle(vals.begin(), vals.end(), rng);
        vector<int> a(n + 1), b(n + 1);
        for (int i = 1; i <= n; i++) a[i] = vals[i - 1];
        for (int i = 1; i <= n; i++) b[i] = vals[n + i - 1];
        ll x = nf_course(n, k0, a, b), y = nf_brute(n, k0, a, b);
        if (x != y) {
            bad++;
            if (bad <= 5) { printf("  n=%d k0=%d a=", n, k0); for (int i = 1; i <= n; i++) printf("%d ", a[i]); printf(" b="); for (int i = 1; i <= n; i++) printf("%d ", b[i]); printf(" course=%lld brute=%lld\n", x, y); }
        }
    }
    printf("题4 糖果药片: %s (%d bad/3000)\n", bad ? "MISMATCH" : "OK", bad);
    if (bad) TOT_FAIL++;
}

// ========== 题5 游戏 ==========
const int MOD5 = 998244353;
vector<ll> gm_course(int n, vector<int>& arr, vector<pair<int,int>>& edges) {
    int m = n >> 1;
    vector<ll> fac(n + 1);
    vector<vector<ll>> c(n + 1, vector<ll>(n + 1, 0));
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % MOD5;
    for (int i = 0; i <= n; i++) { c[i][0] = 1; for (int j = 1; j <= i; j++) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % MOD5; }
    vector<vector<int>> adj(n + 1);
    for (auto& e : edges) { adj[e.first].push_back(e.second); adj[e.second].push_back(e.first); }
    vector<int> sz(n + 1), bel(n + 1, 0);
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
    vector<ll> backup(n + 1);
    // 迭代后序 DFS（避免递归爆栈语义等价）
    vector<int> order;
    function<void(int,int)> dfs = [&](int u, int fa) {
        sz[u] = 1; bel[u] = arr[u]; dp[u][0] = 1;
        for (int v : adj[u]) if (v != fa) {
            dfs(v, u);
            for (int i = 0; i <= min(sz[u] / 2, m); i++) { backup[i] = dp[u][i]; dp[u][i] = 0; }
            for (int l = 0; l <= min(sz[u] / 2, m); l++)
                for (int r = 0; r <= min(sz[v] / 2, m - l); r++)
                    dp[u][l + r] = (dp[u][l + r] + backup[l] * dp[v][r]) % MOD5;
            sz[u] += sz[v];
            bel[u] += bel[v];
        }
        int num = 0;
        // bel[u] 是子树中 arr==1 的数量；对手 = 相反阵营数量
        // 需分别统计 0/1
    };
    // 修正：bel 需要两维。重写为内嵌 lambda 计算
    vector<vector<int>> bel2(n + 1, vector<int>(2, 0));
    vector<vector<ll>> dp2(n + 1, vector<ll>(n + 1, 0));
    vector<ll> backup2(n + 1);
    function<void(int,int)> dfs2 = [&](int u, int fa) {
        sz[u] = 1;
        bel2[u][arr[u]] = 1;
        dp2[u][0] = 1;
        for (int v : adj[u]) if (v != fa) {
            dfs2(v, u);
            for (int i = 0; i <= min(sz[u] / 2, m); i++) { backup2[i] = dp2[u][i]; dp2[u][i] = 0; }
            for (int l = 0; l <= min(sz[u] / 2, m); l++)
                for (int r = 0; r <= min(sz[v] / 2, m - l); r++)
                    dp2[u][l + r] = (dp2[u][l + r] + backup2[l] * dp2[v][r]) % MOD5;
            sz[u] += sz[v];
            bel2[u][0] += bel2[v][0];
            bel2[u][1] += bel2[v][1];
        }
        int num = bel2[u][arr[u] ^ 1];
        for (int i = 1; i <= min(num, m); i++) backup2[i] = dp2[u][i];
        for (int i = 1; i <= min(num, m); i++)
            dp2[u][i] = (dp2[u][i] + backup2[i - 1] * (num - i + 1)) % MOD5;
    };
    dfs2(1, 0);
    vector<ll> g(m + 1), f(m + 1, 0);
    for (int i = 0; i <= m; i++) g[i] = dp2[1][i] * fac[m - i] % MOD5;
    for (int k = 0; k <= m; k++) {
        for (int i = k; i <= m; i++) {
            ll term = c[i][k] * g[i] % MOD5;
            if (((i - k) & 1) == 0) f[k] = (f[k] + term) % MOD5;
            else f[k] = (f[k] + term * (MOD5 - 1)) % MOD5;
        }
    }
    return f;
}
// 暴力：游戏 = A点集 → B点集的配对（双射），共 m! 种
// 题目定义「两场游戏不同：存在A点x，小B在小A选择x那个回合所选的B点不同」
// => 游戏由每个 A 点配哪个 B 点决定，A 点顺序不参与计数
vector<ll> gm_brute(int n, vector<int>& arr, vector<pair<int,int>>& edges) {
    int m = n >> 1;
    vector<vector<int>> adj(n + 1);
    for (auto& e : edges) { adj[e.first].push_back(e.second); adj[e.second].push_back(e.first); }
    // anc[u][v] = v 在 u 的子树里：以 1 为根 DFS 求 tin/tout
    vector<vector<int>> anc(n + 1, vector<int>(n + 1, 0));
    {
        vector<int> tin(n + 1), tout(n + 1);
        int timer = 0;
        function<void(int,int)> dfs = [&](int u, int fa) {
            tin[u] = ++timer;
            for (int v : adj[u]) if (v != fa) dfs(v, u);
            tout[u] = ++timer;
        };
        dfs(1, 0);
        for (int x = 1; x <= n; x++)
            for (int y = 1; y <= n; y++)
                if (x != y && tin[x] <= tin[y] && tin[y] <= tout[x]) anc[x][y] = 1;
    }
    vector<int> A, B;
    for (int i = 1; i <= n; i++) (arr[i] == 0 ? A : B).push_back(i);
    // 枚举 B 的全排列，表示 A[i] 配 B[perm[i]]
    vector<int> perm(B);
    vector<ll> f(m + 1, 0);
    do {
        int non = 0;
        for (int i = 0; i < m; i++) {
            int x = A[i], y = perm[i];
            if (anc[x][y]) non++;        // y 在 x 子树：A 胜
            else if (anc[y][x]) non++;   // x 在 y 子树：B 胜
        }
        f[non] = (f[non] + 1) % MOD5;
    } while (next_permutation(perm.begin(), perm.end()));
    return f;
}
void test5() {
    mt19937 rng(145);
    int bad = 0;
    for (int it = 0; it < 600; it++) {
        int n = 2 * (rng() % 3 + 1); // 2,4,6
        int m = n >> 1;
        // 随机树
        vector<pair<int,int>> edges;
        for (int i = 2; i <= n; i++) {
            int p = rng() % (i - 1) + 1;
            edges.push_back({p, i});
        }
        // 随机归属：恰 m 个 0、m 个 1
        vector<int> arr(n + 1);
        vector<int> tmp;
        for (int i = 0; i < m; i++) tmp.push_back(0);
        for (int i = 0; i < m; i++) tmp.push_back(1);
        shuffle(tmp.begin(), tmp.end(), rng);
        for (int i = 1; i <= n; i++) arr[i] = tmp[i - 1];
        auto f1 = gm_course(n, arr, edges);
        auto f2 = gm_brute(n, arr, edges);
        bool ok = true;
        for (int k = 0; k <= m; k++) if (f1[k] != f2[k]) { ok = false; break; }
        if (!ok) {
            bad++;
            if (bad <= 3) {
                printf("  n=%d arr=", n);
                for (int i = 1; i <= n; i++) printf("%d", arr[i]);
                printf(" course=[");
                for (int k = 0; k <= m; k++) printf("%lld ", f1[k]);
                printf("] brute=[");
                for (int k = 0; k <= m; k++) printf("%lld ", f2[k]);
                printf("]\n");
            }
        }
    }
    printf("题5 游戏: %s (%d bad/600)\n", bad ? "MISMATCH" : "OK", bad);
    if (bad) TOT_FAIL++;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    printf(TOT_FAIL ? "\n=== 有 MISMATCH ===\n" : "\n=== 全过 ===\n");
    return TOT_FAIL ? 1 : 0;
}
