#include <bits/stdc++.h>
using namespace std;

mt19937 rng(12345);

int main() {
    int T = 5000;
    for (int t = 0; t < T; t++) {
        int n = 1 + rng() % 8;
        int drop = rng() % n;  // 舍弃 0..n-1，保证至少留 1 个
        printf("%d %d\n", n, drop);
        for (int i = 0; i < n; i++) printf("%d ", (int)(rng() % 21));
        printf("\n");
        int anyb = 0;
        for (int i = 0; i < n; i++) {
            int b = 1 + (int)(rng() % 21);  // b 全正，杜绝分母 0 的假集
            if (b > 0) anyb = 1;
            printf("%d ", b);
        }
        printf("\n");
        if (!anyb) { /* 不会发生：b 恒 ≥1 */ }
    }
    printf("0 0\n");
    return 0;
}
