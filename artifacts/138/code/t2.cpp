#include <bits/stdc++.h>
using namespace std;

const int MAXN = 251;
const int MAXW = 1001;
const double NA = -1e9;
const double sml = 1e-6;

int n, w;
int weight[MAXN], talent[MAXN];
double value[MAXN];
double dp[MAXW];

bool check(double x) {
    for (int i = 1; i <= n; i++) value[i] = (double)talent[i] - x * weight[i];
    dp[0] = 0;
    for (int j = 1; j <= w; j++) dp[j] = NA;
    for (int i = 1; i <= n; i++) {
        for (int p = w; p >= 0; p--) {
            int j = p + weight[i];
            if (j >= w) {
                dp[w] = max(dp[w], dp[p] + value[i]);
            } else {
                dp[j] = max(dp[j], dp[p] + value[i]);
            }
        }
    }
    return dp[w] >= 0;
}

int main() {
    scanf("%d%d", &n, &w);
    for (int i = 1; i <= n; i++) scanf("%d%d", &weight[i], &talent[i]);
    double l = 0, r = 0, ans = 0;
    for (int i = 1; i <= n; i++) r += talent[i];
    while (l < r && r - l >= sml) {
        double x = (l + r) / 2;
        if (check(x)) {
            ans = x;
            l = x + sml;
        } else {
            r = x - sml;
        }
    }
    printf("%d\n", (int)(ans * 1000));
    return 0;
}
