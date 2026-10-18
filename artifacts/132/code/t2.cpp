#include <bits/stdc++.h>
using namespace std;

// 题2：炮兵阵地
// 课程法：行内合法状态 DFS 生成 + 滚动 dp（memo/dp）
// 暴力法：逐行 DFS 枚举状态子集，检查行间十字攻击

const int MAXN = 100, MAXM = 10;

// ---- 课程法（同 Code02_SoldierPosition2）----
struct Course {
    int n, m, k;
    vector<vector<int>> grid;
    vector<int> sta;
    vector<vector<int>> memo, dp;
    void prepare(int j, int s) {
        if (j >= m) { sta.push_back(s); return; }
        prepare(j + 1, s);
        prepare(j + 3, s | (1 << j));
    }
    int cnt(int i, int s) {
        int ans = 0;
        for (int j = 0; j < m; j++)
            if (((s >> j) & 1) && grid[i][j]) ans++;
        return ans;
    }
    int compute() {
        k = sta.size();
        memo.assign(k, vector<int>(k, 0));
        dp.assign(k, vector<int>(k, 0));
        for (int a = 0; a < k; a++) memo[a][0] = cnt(0, sta[a]);
        for (int i = 1; i < n; i++) {
            for (int a = 0; a < k; a++) {
                int c = cnt(i, sta[a]);
                for (int b = 0; b < k; b++) {
                    dp[a][b] = 0;
                    if ((sta[a] & sta[b]) == 0) {
                        for (int cc = 0; cc < k; cc++) {
                            if ((sta[b] & sta[cc]) == 0 && (sta[a] & sta[cc]) == 0) {
                                dp[a][b] = max(dp[a][b], memo[b][cc] + c);
                            }
                        }
                    }
                }
            }
            swap(memo, dp);
        }
        int ans = 0;
        for (int a = 0; a < k; a++)
            for (int b = 0; b < k; b++)
                ans = max(ans, memo[a][b]);
        return ans;
    }
    int solve(const vector<string>& g) {
        n = g.size(); m = g[0].size();
        grid.assign(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                grid[i][j] = (g[i][j] == 'P' ? 1 : 0);
        sta.clear();
        prepare(0, 0);
        return compute();
    }
};

// ---- 暴力：逐行 DFS ----
int brute_n, brute_m;
vector<int> br_grid;  // 位表示
int best_cnt;

bool attack(int r1, int c1, int r2, int c2) {
    if (r1 == r2) return abs(c1 - c2) <= 2;
    if (c1 == c2) return abs(r1 - r2) <= 2;
    return false;
}

void dfs(int r, int prev1, int prev2, int cnt) {
    if (r == brute_n) { best_cnt = max(best_cnt, cnt); return; }
    for (int s = 0; s < (1 << brute_m); s++) {
        // 行内：与 grid 兼容 + 同行互不攻击（间隔 >= 3）
        bool ok = true;
        for (int c = 0; c < brute_m && ok; c++) {
            if (!((s >> c) & 1)) continue;
            if (!((br_grid[r] >> c) & 1)) { ok = false; break; }
            if (c >= 1 && ((s >> (c - 1)) & 1)) { ok = false; break; }
            if (c >= 2 && ((s >> (c - 2)) & 1)) { ok = false; break; }
        }
        if (!ok) continue;
        // 行间：与上一行、上上行十字攻击检查
        int pop = __builtin_popcount(s);
        bool ok2 = true;
        for (int c = 0; c < brute_m && ok2; c++) {
            if (!((s >> c) & 1)) continue;
            for (int c2 = 0; c2 < brute_m; c2++) {
                if ((prev1 >> c2) & 1) {
                    if (attack(r, c, r - 1, c2)) { ok2 = false; break; }
                }
                if ((prev2 >> c2) & 1) {
                    if (attack(r, c, r - 2, c2)) { ok2 = false; break; }
                }
            }
        }
        if (ok2) dfs(r + 1, s, prev1, cnt + pop);
    }
}

int brute(const vector<string>& g) {
    brute_n = g.size(); brute_m = g[0].size();
    br_grid.assign(brute_n, 0);
    for (int i = 0; i < brute_n; i++)
        for (int j = 0; j < brute_m; j++)
            if (g[i][j] == 'P') br_grid[i] |= (1 << j);
    best_cnt = 0;
    dfs(0, 0, 0, 0);
    return best_cnt;
}

int main() {
    {
        vector<string> g = {"P"};
        printf("固定例 1x1: course=%d brute=%d (期望 1)\n", Course().solve(g), brute(g));
        assert(Course().solve(g) == 1 && brute(g) == 1);
    }
    {
        vector<string> g = {"PP", "PP"};
        printf("固定例 2x2 全P: course=%d brute=%d (期望 2)\n", Course().solve(g), brute(g));
    }
    {
        vector<string> g = {"PPP", "PPP", "PPP"};
        printf("固定例 3x3 全P: course=%d brute=%d (期望 3)\n", Course().solve(g), brute(g));
    }
    mt19937 rnd(20261016);
    int T = 3000, bad = 0;
    for (int t = 0; t < T; t++) {
        int n = rnd() % 6 + 1, m = rnd() % 5 + 1;
        vector<string> g(n);
        for (int i = 0; i < n; i++) {
            g[i] = "";
            for (int j = 0; j < m; j++)
                g[i] += (rnd() % 100 < 80) ? 'P' : 'H';
        }
        int a = Course().solve(g), b = brute(g);
        if (a != b) {
            bad++;
            printf("MISMATCH n=%d m=%d course=%d brute=%d\n", n, m, a, b);
            for (auto& s : g) printf("%s\n", s.c_str());
            if (bad > 5) break;
        }
    }
    printf("随机 %d 组, bad=%d\n", T, bad);
    return bad ? 1 : 0;
}
