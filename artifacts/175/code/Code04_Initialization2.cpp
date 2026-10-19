#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXB = 501;
const int MOD  = 1000000007;
int n, m;

long long pre[MAXB][MAXB];
long long suf[MAXB][MAXB];
long long arr[MAXN];
long long sum[MAXB];

int blen, bnum;
int bi[MAXN];
int bl[MAXB];
int br[MAXB];

void add(int x, int y, long long z) {
    if (x <= blen) {
        for (int i = y; i <= x; i++) {
            pre[x][i] += z;
        }
        for (int i = y; i >= 1; i--) {
            suf[x][i] += z;
        }
    } else {
        for (int i = y; i <= n; i += x) {
            arr[i] += z;
            sum[bi[i]] += z;
        }
    }
}

long long querySum(int l, int r) {
    int lb = bi[l], rb = bi[r];
    long long ans = 0;
    if (lb == rb) {
        for (int i = l; i <= r; i++) {
            ans += arr[i];
        }
    } else {
        for (int i = l; i <= br[lb]; i++) {
            ans += arr[i];
        }
        for (int i = bl[rb]; i <= r; i++) {
            ans += arr[i];
        }
        for (int b = lb + 1; b <= rb - 1; b++) {
            ans += sum[b];
        }
    }
    return ans;
}

long long query(int l, int r) {
    long long ans = querySum(l, r);
    for (int x = 1, lth, rth, num; x <= blen; x++) {
        lth = (l - 1) / x + 1;
        rth = (r - 1) / x + 1;
        num = rth - lth - 1;
        if (lth == rth) {
            ans = ans + pre[x][(r - 1) % x + 1] - pre[x][(l - 1) % x];
        } else {
            ans = ans + suf[x][(l - 1) % x + 1] + pre[x][x] % MOD * num + pre[x][(r - 1) % x + 1];
        }
    }
    return ans % MOD;
}

void prepare() {
    blen = (int)sqrt(n);
    bnum = (n + blen - 1) / blen;
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    for (int b = 1; b <= bnum; b++) {
        bl[b] = (b - 1) * blen + 1;
        br[b] = min(b * blen, n);
        for (int i = bl[b]; i <= br[b]; i++) {
            sum[b] += arr[i];
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
    for (int i = 1, op, x, y, z; i <= m; i++) {
        cin >> op >> x >> y;
        if (op == 1) {
            cin >> z;
            add(x, y, z);
        } else {
            cout << query(x, y) << '\n';
        }
    }
    return 0;
}
