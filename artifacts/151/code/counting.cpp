#include <bits/stdc++.h>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 1000001;

// 所有数字
int arr[MAXN];

// 笛卡尔树需要
int left_[MAXN];
int right_[MAXN];
int stack_[MAXN];

// tmp是动态规划的临时结果
long long tmp[MAXN];

int n, m;

// 动态规划表，n * m <= 10^6，所以临时申请
vector<vector<int>> dp;

void build() {
    for (int i = 1, top = 0, pos; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[stack_[pos]] < arr[i]) {
            pos--;
        }
        if (pos > 0) {
            right_[stack_[pos]] = i;
        }
        if (pos < top) {
            left_[i] = stack_[pos + 1];
        }
        stack_[++pos] = i;
        top = pos;
    }
}

void dfs(int u) {
    if (u == 0) {
        return;
    }
    dfs(left_[u]);
    dfs(right_[u]);
    for (int j = 1; j <= m; j++) {
        tmp[j] = 1LL * dp[left_[u]][j - 1] * dp[right_[u]][j] % MOD;
    }
    for (int j = 1; j <= m; j++) {
        dp[u][j] = (dp[u][j - 1] + tmp[j]) % MOD;
    }
}

long long compute() {
    build();
    // 虽然n * m <= 10^6，但是n和m，单独都是2 * 10^5规模
    // 所以提前准备固定大小的表是不可能的，空间根本接受不了
    // 所以根据此时具体的n和m的大小，临时申请动态规划表，总大小不超过10^6
    dp.assign(n + 1, vector<int>(m + 1, 0));
    for (int j = 0; j <= m; j++) {
        // 没有节点时，不管要求节点值是什么，都默认有1种形态
        // 因为没有节点时，根本不影响任何决策
        dp[0][j] = 1;
    }
    dfs(stack_[1]);
    return dp[stack_[1]][m];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int cases;
    cin >> cases;
    for (int t = 1; t <= cases; t++) {
        cin >> n;
        cin >> m;
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }
        cout << compute() << endl;
    }
    return 0;
}
