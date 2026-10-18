// removing AGC028B 对拍：n 小枚举 n! 消除方案 vs 课程公式
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
const int MAXN = 100001;
int arr[MAXN];
int inv[MAXN];
int sum_[MAXN];
int n;

void build() {
    inv[1] = 1;
    for (int i = 2; i <= n; i++) inv[i] = (int)(MOD - (long long)inv[MOD % i] * (MOD / i) % MOD);
    for (int i = 1; i <= n; i++) sum_[i] = (sum_[i-1] + inv[i]) % MOD;
}
long long compute() {
    build();
    long long ans = 0;
    for (int i = 1; i <= n; i++) ans = (ans + (long long)(sum_[i] + sum_[n - i + 1] - 1) * arr[i]) % MOD;
    for (int i = 1; i <= n; i++) ans = ans * i % MOD;
    return ans;
}

int main() {
    srand(20261021);
    int total = 0, fails = 0;
    for (int round = 0; round < 400; round++) {
        n = 1 + rand() % 7;
        for (int i = 1; i <= n; i++) arr[i] = 1 + rand() % 9;
        long long got = compute();
        // 暴力：枚举消除顺序全排列
        vector<int> perm(n);
        for (int i = 0; i < n; i++) perm[i] = i + 1;
        long long brute = 0;
        vector<int> alive(n + 1, 1);
        sort(perm.begin(), perm.end());
        do {
            vector<int> al(n + 1, 1);
            long long gain = 0;
            for (int step = 0; step < n; step++) {
                int cur = perm[step];
                // 收益 = 消除前 cur 所在连续未消除段（含 cur）的重量和
                int l = cur; while (l > 1 && al[l - 1]) l--;
                int r = cur; while (r < n && al[r + 1]) r++;
                long long seg = 0;
                for (int x = l; x <= r; x++) if (al[x]) seg += arr[x];
                gain += seg;
                al[cur] = 0;
            }
            brute = (brute + gain) % MOD;
        } while (next_permutation(perm.begin(), perm.end()));
        total++;
        if (got != brute) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d n=%d arr:", round, n);
                for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
                printf(" got=%lld brute=%lld\n", got, brute);
            }
        }
    }
    printf("== removing: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
