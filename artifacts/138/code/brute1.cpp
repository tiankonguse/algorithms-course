#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    while (scanf("%d%d", &n, &k) == 2 && (n || k)) {
        vector<double> a(n), b(n);
        for (int i = 0; i < n; i++) scanf("%lf", &a[i]);
        for (int i = 0; i < n; i++) scanf("%lf", &b[i]);
        int keep = n - k;
        double best = 0;
        for (int mask = 0; mask < (1 << n); mask++) {
            if (__builtin_popcount(mask) != keep) continue;
            double sa = 0, sb = 0;
            for (int i = 0; i < n; i++)
                if (mask >> i & 1) { sa += a[i]; sb += b[i]; }
            if (sb <= 0) continue;
            best = max(best, sa / sb);
        }
        printf("%d\n", (int)(100 * best + 0.5));
    }
    return 0;
}
