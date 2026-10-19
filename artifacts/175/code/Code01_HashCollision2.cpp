#include <bits/stdc++.h>

using namespace std;

const int MAXN = 150001;
const int MAXB = 401;
int n, m, blen;
int arr[MAXN];
long long dp[MAXB][MAXB];

long long query(int x, int y) {
    if (x <= blen) {
        return dp[x][y];
    }
    long long ans = 0;
    for (int i = y; i <= n; i += x) {
        ans += arr[i];
    }
    return ans;
}

void update(int i, int v) {
    int delta = v - arr[i];
    arr[i] = v;
    for (int x = 1; x <= blen; x++) {
        dp[x][i % x] += delta;
    }
}

void prepare() {
    blen = (int)sqrt(n);
    for (int x = 1; x <= blen; x++) {
        for (int i = 1; i <= n; i++) {
            dp[x][i % x] += arr[i];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    prepare();
    char op;
    int x, y;
    for (int i = 1; i <= m; i++) {
        cin >> op >> x >> y;
        if (op == 'A') {
            cout << query(x, y) << '\n';
        } else {
            update(x, y);
        }
    }
    return 0;
}
