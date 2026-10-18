#include <bits/stdc++.h>
using namespace std;

// 题5：从上到下挖砖块
// grid[i][j] = 原三角形（第 j 行，右数第 i 列）；挖 grid[i][j] 需正上 grid[i][j-1] 与右上 grid[i-1][j-1]
// 课程法：后缀 max 预处理 + 滚动（compute2）
// 朴素法：枚举上一列 p（compute1）
// 独立暴力：每列枚举挖的块数 j（递减支撑链），直接按题面规则检查

int n, m;
vector<vector<int>> grid;

// 朴素 O(n^3*m)
int naive() {
    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(n + 1, vector<int>(m + 1, 0)));
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int cur = 0;
        for (int j = 0; j <= i; j++) {
            cur += grid[i][j];
            for (int k = (j + 1) * j / 2; k <= m; k++) {
                int mx = 0;
                for (int p = max(0, j - 1); p <= i - 1; p++)
                    mx = max(mx, dp[i - 1][p][k - j]);
                dp[i][j][k] = mx + cur;
                ans = max(ans, dp[i][j][k]);
            }
        }
    }
    return ans;
}

// 课程：后缀 max 优化 + 滚动 O(n^2*m)
int course() {
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    vector<vector<int>> mx(n + 1, vector<int>(m + 1, 0));
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int col = 0; col <= m; col++) {
            int suf = 0;
            for (int row = i - 1; row >= 0; row--) {
                suf = max(suf, dp[row][col]);
                mx[row][col] = suf;
            }
        }
        int cur = 0;
        for (int j = 0; j <= i; j++) {
            cur += grid[i][j];
            for (int k = (j + 1) * j / 2; k <= m; k++) {
                dp[j][k] = mx[max(0, j - 1)][k - j] + cur;
                ans = max(ans, dp[j][k]);
            }
        }
    }
    return ans;
}

// 独立暴力：枚举每列挖 j 块（j_i 递减支撑：j_{i-1} >= j_i - 1），收益 = 该列前 j 块和，总块数 <= m
int brute() {
    vector<int> colPick(n + 1, 0);
    long long best = 0;
    function<void(int, int, int, long long)> dfs = [&](int i, int prev, int cnt, long long sum) {
        // i: 当前列（从底 i=1 到顶 n）
        if (i > n) {
            if (cnt <= m) best = max(best, sum);
            return;
        }
        for (int j = 0; j <= i; j++) {
            // 挖第 i 列前 j 块，需要第 i-1 列前 max(0,j-1) 块已挖
            if (j - 1 > prev) continue;
            int c = 0;
            for (int k = 1; k <= j; k++) c += grid[i][k];
            dfs(i + 1, j, cnt + j, sum + c);
        }
    };
    dfs(1, 0, 0, 0);
    return (int)best;
}

int main() {
    // 固定例：样例 4 2 9 5 / 3 1 7 / 2 4 / 8
    {
        n = 4; m = 8;
        grid.assign(n + 1, vector<int>(n + 1, 0));
        // 读入顺序：for j=1..n: for i=n..j: grid[i][j]
        vector<int> input = {4, 2, 9, 5, 3, 1, 7, 2, 4, 8};
        int p = 0;
        for (int j = 1; j <= n; j++)
            for (int i = n; i >= j; i--)
                grid[i][j] = input[p++];
        printf("固定例样例 m=8: naive=%d course=%d brute=%d\n", naive(), course(), brute());
        printf("固定例样例 m=6: course=%d brute=%d\n", ([](){ m=6; return course(); }()), ([](){ m=6; return brute(); }()));
        m = 8;
    }
    mt19937 rnd(20261016);
    int T = 3000, bad = 0;
    for (int t = 0; t < T; t++) {
        n = rnd() % 7 + 2;
        m = rnd() % 25 + 5;
        grid.assign(n + 1, vector<int>(n + 1, 0));
        for (int j = 1; j <= n; j++)
            for (int i = n; i >= j; i--)
                grid[i][j] = rnd() % 10 + 1;
        int a = naive(), b = course(), c = brute();
        if (a != b || a != c) {
            bad++;
            printf("MISMATCH n=%d m=%d naive=%d course=%d brute=%d\n", n, m, a, b, c);
            if (bad > 5) break;
        }
    }
    printf("随机 %d 组, bad=%d\n", T, bad);
    return bad ? 1 : 0;
}
