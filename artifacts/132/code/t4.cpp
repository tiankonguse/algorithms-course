#include <bits/stdc++.h>
using namespace std;

// 题4：粉刷房子III
// 课程法：pre/suf 前缀最小值优化 + 滚动（minCost4）
// 朴素法：枚举 color（minCost2 逻辑）
// 独立暴力：未涂色房全排列颜色，统计街区

const int NA = INT_MAX;

// 朴素 O(n*t*c^2)
int naive(const vector<int>& house, const vector<vector<int>>& cost, int n, int c, int t) {
    t++;
    vector<vector<int>> memo(t + 1, vector<int>(c + 1, NA)), dp(t + 1, vector<int>(c + 1, NA));
    for (int v = 0; v <= c; v++) memo[0][v] = dp[0][v] = NA;
    for (int j = 1; j <= t; j++)
        for (int v = 0; v <= c; v++)
            memo[j][v] = (j == 1 ? 0 : NA);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= t; j++) {
            for (int v = 0; v <= c; v++) {
                int ans = NA;
                if (house[i] != 0) {
                    if (house[i] == v) ans = memo[j][house[i]];
                    else ans = memo[j - 1][house[i]];
                } else {
                    for (int color = 1; color <= c; color++) {
                        int next = (color == v) ? memo[j][color] : memo[j - 1][color];
                        if (next != NA) ans = min(ans, next + cost[i][color]);
                    }
                }
                dp[j][v] = ans;
            }
        }
        swap(memo, dp);
    }
    int ans = memo[t][0];
    return ans == NA ? -1 : ans;
}

// 课程：pre/suf 优化 O(n*t*c)
int course(const vector<int>& house, const vector<vector<int>>& cost, int n, int c, int t) {
    t++;
    vector<vector<int>> memo(t + 1, vector<int>(c + 1, NA)), dp(t + 1, vector<int>(c + 1, NA));
    for (int v = 0; v <= c; v++) memo[0][v] = dp[0][v] = NA;
    for (int j = 1; j <= t; j++)
        for (int v = 0; v <= c; v++)
            memo[j][v] = (j == 1 ? 0 : NA);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= t; j++) {
            vector<int> pre(c + 2, NA), suf(c + 2, NA);
            pre[0] = suf[c + 1] = NA;
            for (int v = 1; v <= c; v++) {
                pre[v] = pre[v - 1];
                if (memo[j - 1][v] != NA) pre[v] = min(pre[v], memo[j - 1][v] + cost[i][v]);
            }
            for (int v = c; v >= 1; v--) {
                suf[v] = suf[v + 1];
                if (memo[j - 1][v] != NA) suf[v] = min(suf[v], memo[j - 1][v] + cost[i][v]);
            }
            for (int v = 0; v <= c; v++) {
                int ans = NA;
                if (house[i] != 0) {
                    if (house[i] == v) ans = memo[j][house[i]];
                    else ans = memo[j - 1][house[i]];
                } else {
                    if (v == 0) ans = suf[1];
                    else {
                        ans = min(pre[v - 1], suf[v + 1]);
                        if (memo[j][v] != NA) ans = min(ans, memo[j][v] + cost[i][v]);
                    }
                }
                dp[j][v] = ans;
            }
        }
        swap(memo, dp);
    }
    int ans = memo[t][0];
    return ans == NA ? -1 : ans;
}

// 独立暴力：全排列颜色
int brute(const vector<int>& house, const vector<vector<int>>& cost, int n, int c, int t) {
    vector<int> col(n + 1, 0);
    long long best = LLONG_MAX;
    function<void(int)> dfs = [&](int i) {
        if (i > n) {
            int blocks = 1;
            for (int k = 2; k <= n; k++) if (col[k] != col[k - 1]) blocks++;
            if (blocks != t) return;
            long long s = 0;
            for (int k = 1; k <= n; k++) if (house[k] == 0) s += cost[k][col[k]];
            best = min(best, s);
            return;
        }
        if (house[i] != 0) { col[i] = house[i]; dfs(i + 1); }
        else {
            for (int v = 1; v <= c; v++) { col[i] = v; dfs(i + 1); }
        }
    };
    dfs(1);
    return best == LLONG_MAX ? -1 : (int)best;
}

int main() {
    // 固定例：LC1473 官方例1  [0,0,0,0,0] cost 全部 1, t=3 -> ?
    {
        vector<int> house(6, 0);
        vector<vector<int>> cost(6, vector<int>(5, 1));
        printf("固定例 n=5 c=4 t=3 全0 cost=1: naive=%d course=%d brute=%d (期望 3)\n",
            naive(house, cost, 5, 4, 3), course(house, cost, 5, 4, 3), brute(house, cost, 5, 4, 3));
    }
    {
        // LC1473 例1: houses=[0,0,0,0,0], cost = [[1,10],[10,1],[10,1],[1,10],[5,1]], n=5 m=2 target=3 -> 9
        vector<int> house(6, 0);
        vector<vector<int>> cost(6, vector<int>(3, 0));
        cost[1][1] = 1; cost[1][2] = 10;
        cost[2][1] = 10; cost[2][2] = 1;
        cost[3][1] = 10; cost[3][2] = 1;
        cost[4][1] = 1; cost[4][2] = 10;
        cost[5][1] = 5; cost[5][2] = 1;
        printf("固定例 LC1473-1: naive=%d course=%d brute=%d (期望 9)\n",
            naive(house, cost, 5, 2, 3), course(house, cost, 5, 2, 3), brute(house, cost, 5, 2, 3));
    }
    mt19937 rnd(20261016);
    int T = 4000, bad = 0;
    for (int t = 0; t < T; t++) {
        int n = rnd() % 5 + 2;
        int c = rnd() % 4 + 2;
        int target = rnd() % n + 1;
        vector<int> house(n + 1, 0);
        vector<vector<int>> cost(n + 1, vector<int>(c + 1, 0));
        for (int i = 1; i <= n; i++) {
            if (rnd() % 100 < 50) house[i] = rnd() % c + 1;
            for (int v = 1; v <= c; v++) cost[i][v] = rnd() % 8 + 1;
        }
        int a = naive(house, cost, n, c, target);
        int b = course(house, cost, n, c, target);
        int bb = brute(house, cost, n, c, target);
        if (a != b || a != bb) {
            bad++;
            printf("MISMATCH n=%d c=%d t=%d naive=%d course=%d brute=%d\n", n, c, target, a, b, bb);
            for (int i = 1; i <= n; i++) printf("%d ", house[i]);
            printf("\ncost:\n");
            for (int i = 1; i <= n; i++) {
                for (int v = 1; v <= c; v++) printf("%d ", cost[i][v]);
                printf("\n");
            }
            if (bad == 1) {
                // 打印 brute 最优方案
                int bn = n, bc = c, bt = target;
                vector<int> bcol(n + 1, 0), bestcol(n + 1, 0);
                long long bestv = LLONG_MAX;
                function<void(int)> dfs2 = [&](int i) {
                    if (i > bn) {
                        int blocks = 1;
                        for (int k = 2; k <= bn; k++) if (bcol[k] != bcol[k - 1]) blocks++;
                        if (blocks != bt) return;
                        long long s = 0;
                        for (int k = 1; k <= bn; k++) s += cost[k][bcol[k]];
                        if (s < bestv) { bestv = s; bestcol = bcol; }
                        return;
                    }
                    if (house[i] != 0) { bcol[i] = house[i]; dfs2(i + 1); }
                    else { for (int v = 1; v <= bc; v++) { bcol[i] = v; dfs2(i + 1); } }
                };
                dfs2(1);
                printf("brute best=%lld col:", bestv);
                for (int k = 1; k <= bn; k++) printf("%d ", bestcol[k]);
                printf("\n");
                // 数街区
                int blocks = 1;
                for (int k = 2; k <= bn; k++) if (bestcol[k] != bestcol[k - 1]) blocks++;
                printf("blocks=%d\n", blocks);
            }
            if (bad > 5) break;
        }
    }
    printf("随机 %d 组, bad=%d\n", T, bad);
    return bad ? 1 : 0;
}
