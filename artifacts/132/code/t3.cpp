#include <bits/stdc++.h>
using namespace std;

// 题3：还原数组的方法数
// 约束：arr[i] <= max(arr[i-1], arr[i+1])，边界 arr[1]<=arr[2], arr[n]<=arr[n-1]
// 课程法：suf0/pre1 前缀和优化 + 滚动（m 参数化以便小值域对拍）
// 朴素法：三段枚举 cur（O(n*m^2)）
// 独立暴力：全排列填值，直接按题面约束检查

const int MOD = 998244353;

// 朴素：compute3 版（滚动 + 枚举）
int naive(const vector<int>& arr, int m) {
    int n = (int)arr.size() - 1;  // arr[1..n]，arr[0] 占位
    vector<vector<int>> memo(m + 1, vector<int>(2, 0)), dp(m + 1, vector<int>(2, 0));
    for (int v = 0; v <= m; v++) { memo[v][0] = 0; memo[v][1] = 1; }
    for (int i = 1; i <= n; i++) {
        for (int v = 0; v <= m; v++) {
            for (int s = 0; s <= 1; s++) {
                int ans = 0;
                if (arr[i] != 0) {
                    if (arr[i] >= v || s == 1) ans = memo[arr[i]][arr[i] > v ? 0 : 1];
                } else {
                    for (int cur = v + 1; cur <= m; cur++) ans = (ans + memo[cur][0]) % MOD;
                    if (v != 0) ans = (ans + memo[v][1]) % MOD;
                    if (s == 1) for (int cur = 1; cur < v; cur++) ans = (ans + memo[cur][1]) % MOD;
                }
                dp[v][s] = ans;
            }
        }
        swap(memo, dp);
    }
    return memo[0][1];
}

// 课程：suf0/pre1 优化 + 滚动
int course(const vector<int>& arr, int m) {
    int n = (int)arr.size() - 1;  // arr[1..n]
    vector<vector<int>> memo(m + 1, vector<int>(2, 0)), dp(m + 1, vector<int>(2, 0));
    vector<int> suf0(m + 1, 0), pre1(m + 1, 0);
    for (int v = 0; v <= m; v++) { memo[v][0] = 0; memo[v][1] = 1; }
    for (int i = 1; i <= n; i++) {
        suf0[m] = memo[m][0];
        for (int v = m - 1; v >= 0; v--) suf0[v] = (suf0[v + 1] + memo[v][0]) % MOD;
        pre1[1] = memo[1][1];
        for (int v = 2; v <= m; v++) pre1[v] = (pre1[v - 1] + memo[v][1]) % MOD;
        for (int v = 0; v <= m; v++) {
            for (int s = 0; s <= 1; s++) {
                int ans = 0;
                if (arr[i] != 0) {
                    if (arr[i] >= v || s == 1) ans = memo[arr[i]][arr[i] > v ? 0 : 1];
                } else {
                    if (v + 1 <= m) ans = (ans + suf0[v + 1]) % MOD;
                    if (v != 0) ans = (ans + memo[v][1]) % MOD;
                    if (s == 1 && v - 1 >= 0) ans = (ans + pre1[v - 1]) % MOD;
                }
                dp[v][s] = ans;
            }
        }
        swap(memo, dp);
    }
    return memo[0][1];
}

// 独立暴力：全排列填值
int brute(const vector<int>& arr, int m) {
    int n = (int)arr.size() - 1;  // arr[1..n]
    vector<int> a(n + 2, 0);  // 1..n，a[0]=a[n+1]=哨兵（约束只查 2..n-1？不，边界特殊）
    long long cnt = 0;
    function<void(int)> dfs = [&](int pos) {
        if (pos > n) {
            // 检查约束
            for (int i = 2; i <= n - 1; i++) {
                if (a[i] > max(a[i - 1], a[i + 1])) return;
            }
            if (a[1] > a[2]) return;
            if (a[n] > a[n - 1]) return;
            cnt++;
            return;
        }
        if (arr[pos] != 0) {
            a[pos] = arr[pos];
            dfs(pos + 1);
        } else {
            for (int v = 1; v <= m; v++) { a[pos] = v; dfs(pos + 1); }
        }
    };
    dfs(1);
    return (int)(cnt % MOD);
}

int main() {
    // 固定例
    {
        vector<int> arr = {0, 0, 0};
        printf("固定例 [0,0,0] m=3: naive=%d course=%d brute=%d\n", naive(arr, 3), course(arr, 3), brute(arr, 3));
    }
    {
        vector<int> arr = {1, 0, 0};
        printf("固定例 [1,0,0] m=3: naive=%d course=%d brute=%d\n", naive(arr, 3), course(arr, 3), brute(arr, 3));
    }
    mt19937 rnd(20261016);
    int T = 5000, bad = 0;
    for (int t = 0; t < T; t++) {
        int n = rnd() % 5 + 3;
        int m = rnd() % 6 + 2;
        vector<int> arr(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            if (rnd() % 100 < 60) arr[i] = rnd() % m + 1;
        }
        int a = naive(arr, m), b = course(arr, m), c = brute(arr, m);
        if (a != b || a != c) {
            bad++;
            printf("MISMATCH n=%d m=%d naive=%d course=%d brute=%d\n", n, m, a, b, c);
            for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
            printf("\n");
            if (bad > 5) break;
        }
    }
    printf("随机 %d 组, bad=%d\n", T, bad);
    return bad ? 1 : 0;
}
