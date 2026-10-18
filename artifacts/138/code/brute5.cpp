#include <bits/stdc++.h>
using namespace std;

// 暴力：枚举 1..n 中恰好 k 个的闭包子集（选 v 必选其父），最大化战斗/花费
int main() {
    int k, n;
    scanf("%d%d", &k, &n);
    vector<int> c(n + 1), s(n + 1), pa(n + 1);
    for (int i = 1; i <= n; i++) scanf("%d%d%d", &c[i], &s[i], &pa[i]);
    double best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        if (__builtin_popcount(mask) != k) continue;
        bool ok = true;
        for (int i = 1; i <= n && ok; i++)
            if (mask >> (i - 1) & 1) {
                int p = pa[i];
                if (p != 0 && !(mask >> (p - 1) & 1)) ok = false;
            }
        if (!ok) continue;
        long long sc = 0, ss = 0;
        for (int i = 1; i <= n; i++)
            if (mask >> (i - 1) & 1) { sc += c[i]; ss += s[i]; }
        if (sc > 0) best = max(best, (double)ss / sc);
    }
    printf("%.3f\n", best);
    return 0;
}
