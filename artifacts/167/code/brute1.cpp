#include <bits/stdc++.h>
using namespace std;
const long long BAS = 10000019LL, MOD = 1000000007LL;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, k, q;
    cin >> n >> k;
    map<int, pair<int,int>> goods; // id -> (v,w)
    for (int i = 1; i <= n; i++) { int v, w; cin >> v >> w; goods[i] = {v, w}; }
    cin >> q;
    int nxt = n + 1;
    for (int i = 0; i < q; i++) {
        int op; cin >> op;
        if (op == 1) { int v, w; cin >> v >> w; goods[nxt++] = {v, w}; }
        else if (op == 2) { int x; cin >> x; goods.erase(x); }
        else {
            vector<long long> dp(k + 1, 0);
            for (auto& pr : goods) {
                int v = pr.second.first, w = pr.second.second;
                for (int m = k; m >= w; m--)
                    dp[m] = max(dp[m], dp[m - w] + v);
            }
            long long ret = 0, base = 1;
            for (int m = 1; m <= k; m++) {
                ret = (ret + dp[m] * base) % MOD;
                base = (base * BAS) % MOD;
            }
            cout << ret << '\n';
        }
    }
    return 0;
}
