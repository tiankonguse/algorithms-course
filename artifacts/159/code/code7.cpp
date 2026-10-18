#include <bits/stdc++.h>

using namespace std;

const int MAXN = 50001;
const int MAXT = MAXN * 20;
const int BIT = 30;
const int MOD = 1000000007;
const int INV2 = 500000004;
int n, k;
int arr[MAXN];
int tree[MAXT][2];
int pass[MAXT];
int cnt = 1;
int sum[MAXT][BIT + 1];

void insert(int num) {
    int cur = 1;
    pass[1]++;
    for (int b = BIT; b >= 0; b--) {
        int path = (num >> b) & 1;
        if (!tree[cur][path]) {
            tree[cur][path] = ++cnt;
        }
        cur = tree[cur][path];
        pass[cur]++;
    }
}

void dfs(int i, int h, int s) {
    if (!i) {
        return;
    }
    if (!h) {
        for (int j = 0; j <= BIT; j++) {
            if ((s >> j) & 1) {
                sum[i][j] = pass[i];
            }
        }
    } else {
        dfs(tree[i][0], h - 1, s);
        dfs(tree[i][1], h - 1, s | (1 << (h - 1)));
        for (int j = 0; j <= BIT; j++) {
            sum[i][j] = sum[tree[i][0]][j] + sum[tree[i][1]][j];
        }
    }
}

long long moreEqual(int x) {
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        int num = arr[i];
        int cur = 1;
        for (int b = BIT; b >= 0; b--) {
            int path = (num >> b) & 1;
            int best = path ^ 1;
            int xpath = (x >> b) & 1;
            if (!xpath) {
                ans += pass[tree[cur][best]];
                cur = tree[cur][path];
            } else {
                cur = tree[cur][best];
            }
            if (!cur) {
                break;
            }
        }
        ans += pass[cur];
    }
    if (x == 0) {
        ans -= n;
    }
    return ans / 2;
}

int maxKth() {
    int l = 0, r = 1 << BIT, ans = 0;
    while (l <= r) {
        int m = (l + r) >> 1;
        if (moreEqual(m) >= k) {
            ans = m;
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return ans;
}

long long compute() {
    int kth = maxKth();
    long long ans = 0;
    for (int i = 1, cur; i <= n; i++) {
        cur = 1;
        for (int b = BIT; b >= 0; b--) {
            int path = (arr[i] >> b) & 1;
            int best = path ^ 1;
            int kpath = (kth >> b) & 1;
            if (!kpath) {
                if (tree[cur][best]) {
                    for (int j = 0; j <= BIT; j++) {
                        if ((arr[i] >> j) & 1) {
                            ans = (ans + ((long long)pass[tree[cur][best]] - sum[tree[cur][best]][j]) * (1LL << j)) % MOD;
                        } else {
                            ans = (ans + ((long long)sum[tree[cur][best]][j]) * (1LL << j)) % MOD;
                        }
                    }
                }
                cur = tree[cur][path];
            } else {
                cur = tree[cur][best];
            }
            if (!cur) {
                break;
            }
        }
        if (cur) {
            ans = (ans + (long long)pass[cur] * kth) % MOD;
        }
    }
    ans = ans * INV2 % MOD;    
    ans = ((ans - ((moreEqual(kth) - k) * kth) % MOD) % MOD + MOD) % MOD;
    return ans;
}

void prepare() {
    for (int i = 1; i <= n; i++) {
        insert(arr[i]);
    }
    dfs(tree[1][0], BIT, 0);
    dfs(tree[1][1], BIT, 1 << BIT);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    if (!k) {
        cout << 0 << "\n";
    } else {
        prepare();
        cout << compute() << "\n";
    }
    return 0;
}
