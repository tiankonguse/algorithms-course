#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, w;
    scanf("%d%d", &n, &w);
    vector<int> wt(n), tl(n);
    for (int i = 0; i < n; i++) scanf("%d%d", &wt[i], &tl[i]);
    double best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int sw = 0, st = 0;
        for (int i = 0; i < n; i++)
            if (mask >> i & 1) { sw += wt[i]; st += tl[i]; }
        if (sw >= w && sw > 0) best = max(best, (double)st / sw);
    }
    printf("%d\n", (int)(best * 1000));
    return 0;
}
