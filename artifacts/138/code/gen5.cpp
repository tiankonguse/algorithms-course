#include <bits/stdc++.h>
using namespace std;

mt19937 rng(13579);

int main() {
    int T = 2000;
    for (int t = 0; t < T; t++) {
        int n = 1 + rng() % 8;
        int k = 1 + rng() % n;
        printf("%d %d\n", k, n);
        for (int i = 1; i <= n; i++) {
            int cost = 1 + (int)(rng() % 20);
            int strength = 1 + (int)(rng() % 30);
            int p = rng() % i;  // 0..i-1，保证父编号小于 i，树形
            printf("%d %d %d\n", cost, strength, p);
        }
    }
    return 0;
}
