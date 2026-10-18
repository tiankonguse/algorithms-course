#include <bits/stdc++.h>
using namespace std;

mt19937 rng(54321);

int main() {
    int T = 3000;
    for (int t = 0; t < T; t++) {
        int n = 1 + rng() % 9;
        int w = 1 + rng() % 20;
        printf("%d %d\n", n, w);
        for (int i = 0; i < n; i++) {
            int wt = 1 + (int)(rng() % 15);
            int tl = 1 + (int)(rng() % 20);
            printf("%d %d\n", wt, tl);
        }
    }
    return 0;
}
