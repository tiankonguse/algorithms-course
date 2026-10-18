// periodni P6453 对拍：小直方图暴力枚举填数 vs 课程 dp
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
const int MAXN = 501;
const int MAXH = 1000000;
int arr[MAXN];
int fac[MAXH + 1], inv[MAXH + 1];
int left_[MAXN], right_[MAXN], stack_[MAXN];
int size_[MAXN];
int dp[MAXN][MAXN];
int tmp[MAXN];
int n, k;

int power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) { if (p & 1) ans = ans * x % MOD; x = x * x % MOD; p >>= 1; }
    return (int) ans;
}
int c(int n, int k) { return n < k ? 0 : (int)((long long)fac[n] * inv[k] % MOD * inv[n - k] % MOD); }
void build() {
    fac[0] = fac[1] = inv[0] = 1;
    for (int i = 2; i <= MAXH; i++) fac[i] = (int)((long long)fac[i-1] * i % MOD);
    inv[MAXH] = power(fac[MAXH], MOD - 2);
    for (int i = MAXH - 1; i >= 1; i--) inv[i] = (int)((long long)inv[i+1] * (i + 1) % MOD);
    for (int i = 1, top = 0, pos; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[stack_[pos]] > arr[i]) pos--;
        if (pos > 0) right_[stack_[pos]] = i;
        if (pos < top) left_[i] = stack_[pos + 1];
        stack_[++pos] = i; top = pos;
    }
}
void dfs(int u, int fa) {
    if (u == 0) return;
    dfs(left_[u], u); dfs(right_[u], u);
    size_[u] = size_[left_[u]] + size_[right_[u]] + 1;
    for (int i = 0; i <= k; i++) tmp[i] = 0;
    for (int l = 0; l <= min(size_[left_[u]], k); l++)
        for (int r = 0; r <= min(size_[right_[u]], k - l); r++)
            tmp[l + r] = (tmp[l + r] + (long long)dp[left_[u]][l] * dp[right_[u]][r] % MOD) % MOD;
    for (int i = 0; i <= min(size_[u], k); i++)
        for (int p = 0; p <= i; p++)
            dp[u][i] = (dp[u][i] + (long long)c(size_[u] - p, i - p) * c(arr[u] - arr[fa], i - p) % MOD
                        * fac[i - p] % MOD * tmp[p] % MOD) % MOD;
}
int compute() {
    build();
    dp[0][0] = 1;
    dfs(stack_[1], 0);
    return dp[stack_[1]][k];
}

// 暴力：格子 (x, y)，x=1..n, y=1..arr[x]
bool validPos(const vector<pair<int,int>>& cells) {
    int K = cells.size();
    for (int a = 0; a < K; a++)
        for (int b = a + 1; b < K; b++) {
            int ax = cells[a].first, ay = cells[a].second;
            int bx = cells[b].first, by = cells[b].second;
            if (ax == bx) return false; // 同列
            if (ay == by) { // 同行：中间断开不违规，连通才违规
                int lo = min(ax, bx), hi = max(ax, bx);
                bool connected = true;
                for (int x = lo; x <= hi; x++) if (arr[x] < ay) { connected = false; break; }
                if (connected) return false;
            }
        }
    return true;
}

int main() {
    srand(20261020);
    int total = 0, fails = 0;
    for (int round = 0; round < 200; round++) {
        n = 1 + rand() % 5;
        k = 1 + rand() % 4;
        for (int i = 1; i <= n; i++) arr[i] = 1 + rand() % 6;
        if (k > 10) { round--; continue; } // 控制暴力规模
        memset(left_, 0, sizeof(left_)); memset(right_, 0, sizeof(right_));
        memset(dp, 0, sizeof(dp)); memset(size_, 0, sizeof(size_));
        long long got = compute();
        // 暴力：枚举所有 k 元格子组合
        vector<pair<int,int>> all;
        for (int x = 1; x <= n; x++)
            for (int y = 1; y <= arr[x]; y++)
                all.push_back({x, y});
        long long cnt = 0;
        if ((int)all.size() >= k) {
            vector<int> idx(all.size());
            for (size_t i = 0; i < all.size(); i++) idx[i] = i;
            vector<int> sel(k);
            function<void(int,int)> rec = [&](int start, int depth) {
                if (depth == k) {
                    vector<pair<int,int>> cells;
                    for (int i = 0; i < k; i++) cells.push_back(all[sel[i]]);
                    if (validPos(cells)) cnt++;
                    return;
                }
                for (size_t i = start; i + (k - depth) <= all.size(); i++) {
                    sel[depth] = i;
                    rec(i + 1, depth + 1);
                }
            };
            rec(0, 0);
        }
        cnt %= MOD;
        total++;
        if (got != cnt) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d n=%d k=%d arr:", round, n, k);
                for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
                printf(" got=%lld brute=%lld\n", got, cnt);
            }
        }
    }
    printf("== periodni: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
