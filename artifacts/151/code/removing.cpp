#include <bits/stdc++.h>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 100001;

// 所有数字
int arr[MAXN];

// 连续数字逆元表
int inv[MAXN];

// sum[i] = (1/1 + 1/2 + 1/3 + ... + 1/i)，%MOD意义下的余数
int sum_[MAXN];

int n;

void build() {
    inv[1] = 1;
    for (int i = 2; i <= n; i++) {
        inv[i] = (int) (MOD - (long long) inv[MOD % i] * (MOD / i) % MOD);
    }
    for (int i = 1; i <= n; i++) {
        sum_[i] = (sum_[i - 1] + inv[i]) % MOD;
    }
}

long long compute() {
    build();
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        ans = (ans + (long long) (sum_[i] + sum_[n - i + 1] - 1) * arr[i]) % MOD;
    }
    for (int i = 1; i <= n; i++) {
        ans = ans * i % MOD;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    cout << compute() << endl;
    return 0;
}
