#include <bits/stdc++.h>
using namespace std;
// 背包 P9140 两次转圈法：基准=性价比最高物品，dp[r]=替换基准后的最大价值补偿
int n, m;
int x, y;
long long v[100001], c[100001];
long long dp[100001];
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }
int main() {
    cin >> n >> m;
    double best = 0;
    for (int i = 1; i <= n; i++) {
        cin >> v[i] >> c[i];
        double ratio = (double)c[i] / v[i];
        if (ratio > best) { best = ratio; x = v[i]; y = c[i]; }
    }
    fill(dp, dp + x, LLONG_MIN);
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        if (v[i] == x) continue;
        int d = gcd(v[i], x);
        for (int j = 0; j < d; j++) {
            for (int cur = j, next, circle = 0; circle < 2; circle += cur == j ? 1 : 0) {
                next = (cur + v[i]) % x;
                if (dp[cur] != LLONG_MIN) {
                    dp[next] = max(dp[next], dp[cur] - ((cur + v[i]) / x) * (long long)y + c[i]);
                }
                cur = next;
            }
        }
    }
    long long jobv;
    for (int i = 1; i <= m; i++) {
        cin >> jobv;
        int r = (int)(jobv % x);
        if (dp[r] == LLONG_MIN) cout << -1 << endl;
        else cout << jobv / x * (long long)y + dp[r] << endl;
    }
    return 0;
}
