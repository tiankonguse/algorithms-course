#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXB = 401;
int t, n, q, blen;
int arr[MAXN];
long long f[MAXB][MAXN];
long long g[MAXB][MAXN];

long long query(int s, int d, int k) {
    long long ans = 0;
    if (d <= blen) {
        ans = g[d][s];
        if (s + d * k <= n) {
            ans -= g[d][s + d * k] + f[d][s + d * k] * k;
        }
    } else {
        for (int i = 1; i <= k; i++) {
            ans += 1LL * arr[s + (i - 1) * d] * i;
        }
    }
    return ans;
}

void prepare() {
    blen = (int)sqrt(n);
    for (int d = 1; d <= blen; d++) {
        for (int i = n; i >= 1; i--) {
            f[d][i] = arr[i] + (i + d > n ? 0 : f[d][i + d]);
        }
    }
    for (int d = 1; d <= blen; d++) {
        for (int i = n; i >= 1; i--) {
            g[d][i] = f[d][i] + (i + d > n ? 0 : g[d][i + d]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> t;
    for (int c = 1; c <= t; c++) {
        cin >> n >> q;
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }
        prepare();
        for (int i = 1, s, d, k; i <= q; i++) {
            cin >> s >> d >> k;
            cout << query(s, d, k) << '\n';
        }
    }
    return 0;
}
