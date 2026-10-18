#include <bits/stdc++.h>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 501;
const int MAXH = 1000000;

// 所有数字
int arr[MAXN];

// 阶乘余数表
int fac[MAXH + 1];

// 阶乘逆元表
int inv[MAXH + 1];

// 笛卡尔树需要
int left_[MAXN];
int right_[MAXN];
int stack_[MAXN];

// dfs需要
int size_[MAXN];

// dp是动态规划表
int dp[MAXN][MAXN];

// tmp是动态规划的临时结果
int tmp[MAXN];

int n, k;

int power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if ((p & 1) == 1) {
            ans = ans * x % MOD;
        }
        x = x * x % MOD;
        p >>= 1;
    }
    return (int) ans;
}

int c(int n, int k) {
    return n < k ? 0 : (int) ((long long) fac[n] * inv[k] % MOD * inv[n - k] % MOD);
}

void build() {
    fac[0] = fac[1] = inv[0] = 1;
    for (int i = 2; i <= MAXH; i++) {
        fac[i] = (int) ((long long) fac[i - 1] * i % MOD);
    }
    inv[MAXH] = power(fac[MAXH], MOD - 2);
    for (int i = MAXH - 1; i >= 1; i--) {
        inv[i] = (int) ((long long) inv[i + 1] * (i + 1) % MOD);
    }
    for (int i = 1, top = 0, pos; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[stack_[pos]] > arr[i]) {
            pos--;
        }
        if (pos > 0) {
            right_[stack_[pos]] = i;
        }
        if (pos < top) {
            left_[i] = stack_[pos + 1];
        }
        stack_[++pos] = i;
        top = pos;
    }
}

void dfs(int u, int fa) {
    if (u == 0) {
        return;
    }
    dfs(left_[u], u);
    dfs(right_[u], u);
    size_[u] = size_[left_[u]] + size_[right_[u]] + 1;
    for (int i = 0; i <= k; i++) {
        tmp[i] = 0;
    }
    // 所有dfs过程都算上，这一部分的总复杂度O(n^2)
    for (int l = 0; l <= min(size_[left_[u]], k); l++) {
        for (int r = 0; r <= min(size_[right_[u]], k - l); r++) {
            tmp[l + r] = (tmp[l + r] + (long long) dp[left_[u]][l] * dp[right_[u]][r] % MOD) % MOD;
        }
    }
    // 所有dfs过程都算上，这一部分的总复杂度O(min(n的3次方, n * k平方))
    for (int i = 0; i <= min(size_[u], k); i++) {
        for (int p = 0; p <= i; p++) {
            dp[u][i] = (dp[u][i] + (long long) c(size_[u] - p, i - p) * c(arr[u] - arr[fa], i - p) % MOD
                        * fac[i - p] % MOD * tmp[p] % MOD) % MOD;
        }
    }
}

int compute() {
    build();
    dp[0][0] = 1;
    dfs(stack_[1], 0);
    return dp[stack_[1]][k];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    cin >> k;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    cout << compute() << endl;
    return 0;
}
