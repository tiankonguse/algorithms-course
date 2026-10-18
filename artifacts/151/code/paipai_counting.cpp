// counting CF1748E 对拍：小 n,m 暴力枚举 B 验证最左端最大值位置一致
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
const int MAXN = 1005;
int arr[MAXN], left_[MAXN], right_[MAXN], stack_[MAXN];
long long tmp[MAXN];
int n, m;
vector<vector<int>> dp;

void build() {
    for (int i = 1, top = 0, pos; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[stack_[pos]] < arr[i]) pos--;
        if (pos > 0) right_[stack_[pos]] = i;
        if (pos < top) left_[i] = stack_[pos + 1];
        stack_[++pos] = i;
        top = pos;
    }
}
void dfs(int u) {
    if (u == 0) return;
    dfs(left_[u]); dfs(right_[u]);
    for (int j = 1; j <= m; j++) tmp[j] = 1LL * dp[left_[u]][j-1] * dp[right_[u]][j] % MOD;
    for (int j = 1; j <= m; j++) dp[u][j] = (dp[u][j-1] + tmp[j]) % MOD;
}
long long compute() {
    build();
    dp.assign(n + 1, vector<int>(m + 1, 0));
    for (int j = 0; j <= m; j++) dp[0][j] = 1;
    dfs(stack_[1]);
    return dp[stack_[1]][m];
}

// 暴力：最左端最大值位置
int leftMaxPos(const vector<int>& v, int l, int r) {
    int best = l;
    for (int i = l + 1; i <= r; i++) if (v[i] > v[best]) best = i;
    return best;
}
int leftMaxPos(const int* v, int l, int r) {
    int best = l;
    for (int i = l + 1; i <= r; i++) if (v[i] > v[best]) best = i;
    return best;
}

int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 400; round++) {
        n = 2 + rand() % 4;   // 2..5
        m = 2 + rand() % 3;   // 2..4
        for (int i = 1; i <= n; i++) arr[i] = 1 + rand() % m;
        // 课程结果
        memset(left_, 0, sizeof(left_)); memset(right_, 0, sizeof(right_));
        long long got = compute();
        // 暴力：枚举 B ∈ [1,m]^n
        long long cnt = 0;
        vector<int> B(n + 1);
        function<void(int)> rec = [&](int pos) {
            if (pos > n) {
                for (int l = 1; l <= n; l++)
                    for (int r = l; r <= n; r++)
                        if (leftMaxPos(arr, l, r) != leftMaxPos(B, l, r)) return;
                cnt++;
                return;
            }
            for (int v = 1; v <= m; v++) { B[pos] = v; rec(pos + 1); }
        };
        rec(1);
        cnt %= MOD;
        total++;
        if (got != cnt) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d n=%d m=%d A:", round, n, m);
                for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
                printf(" got=%lld brute=%lld\n", got, cnt);
            }
        }
    }
    printf("== counting: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
