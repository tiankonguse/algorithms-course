// 题2 LC2312 平衡子序列最大和（树状数组值域 max）+ 题4 LC2370 最长理想子序列（线段树 26）
// 页面代码复现（dp-fenwick-value-max.html）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/* ===== 题2：树状数组维护前缀最大值（页面代码） ===== */
const int M2 = 100001;
ll sortv[M2], treeMax[M2];
int m2, n2;

int rankv2(ll v) {
    int left = 1, right = m2, mid;
    int ans = 0;
    while (left <= right) {
        mid = (left + right) / 2;
        if (sortv[mid] <= v) { ans = mid; left = mid + 1; }
        else right = mid - 1;
    }
    return ans;
}

void update2(int i, ll v) {
    while (i <= m2) {
        treeMax[i] = max(treeMax[i], v);
        i += i & -i;
    }
}

ll query2(int i) {
    ll ans = LLONG_MIN;
    while (i > 0) {
        ans = max(ans, treeMax[i]);
        i -= i & -i;
    }
    return ans;
}

void build2(const vector<ll>& nums) {
    n2 = nums.size();
    for (int i = 1, j = 0; i <= n2; i++, j++) sortv[i] = nums[j] - j;
    sort(sortv + 1, sortv + n2 + 1);
    m2 = 1;
    for (int i = 2; i <= n2; i++) if (sortv[m2] != sortv[i]) sortv[++m2] = sortv[i];
    for (int i = 1; i <= m2; i++) treeMax[i] = LLONG_MIN;
}

ll maxBalancedSubsequenceSum(const vector<ll>& nums) {
    build2(nums);
    ll pre;
    for (int i = 0, k; i < n2; i++) {
        k = rankv2(nums[i] - i);
        pre = query2(k);
        if (pre < 0) update2(k, nums[i]);
        else update2(k, pre + nums[i]);
    }
    return query2(m2);
}

/* ===== 题2 暴力 ===== */
ll brute2(const vector<ll>& nums) {
    int n = nums.size();
    ll ans = LLONG_MIN;
    // dp[i] = nums[i] + max{dp[j]: j<i, nums[j]-j <= nums[i]-i}
    vector<ll> dp(n);
    for (int i = 0; i < n; i++) {
        dp[i] = nums[i];
        for (int j = 0; j < i; j++) {
            if (nums[j] - j <= nums[i] - i) dp[i] = max(dp[i], dp[j] + nums[i]);
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}

/* ===== 题4：线段树 26 槽（页面代码） ===== */
int segmax[(26 + 1) << 2];

void up4(int i) {
    segmax[i] = max(segmax[i << 1], segmax[i << 1 | 1]);
}

void update4(int jobi, int jobv, int l, int r, int i) {
    if (l == r && jobi == l) {
        segmax[i] = jobv;
    } else {
        int m = (l + r) >> 1;
        if (jobi <= m) update4(jobi, jobv, l, m, i << 1);
        else update4(jobi, jobv, m + 1, r, i << 1 | 1);
        up4(i);
    }
}

int query4(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) return segmax[i];
    int m = (l + r) >> 1;
    int ans = 0;
    if (jobl <= m) ans = max(ans, query4(jobl, jobr, l, m, i << 1));
    if (jobr > m) ans = max(ans, query4(jobl, jobr, m + 1, r, i << 1 | 1));
    return ans;
}

int longestIdealString(const string& s, int k) {
    memset(segmax, 0, sizeof(segmax));
    int v, p;
    int ans = 0;
    for (char cha : s) {
        v = cha - 'a' + 1;
        p = query4(max(v - k, 1), min(v + k, 26), 1, 26, 1);
        ans = max(ans, p + 1);
        update4(v, p + 1, 1, 26, 1);
    }
    return ans;
}

/* ===== 题4 暴力 ===== */
int brute4(const string& s, int k) {
    int n = s.size();
    vector<int> dp(n, 1);
    int ans = 1;
    for (int i = 0; i < n; i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++) {
            if (abs((int)s[i] - (int)s[j]) <= k) dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}

int main() {
    mt19937 rng(20261016);
    // 题2 对拍
    int cases = 4000;
    for (int tc = 1; tc <= cases; tc++) {
        int n = rng() % 12 + 1;
        vector<ll> nums(n);
        for (int i = 0; i < n; i++) nums[i] = (ll)(rng() % 41) - 20;
        ll a = maxBalancedSubsequenceSum(nums);
        ll b = brute2(nums);
        if (a != b) {
            printf("MISMATCH t2 tc=%d n=%d opt=%lld brute=%lld\n", tc, n, a, b);
            for (ll x : nums) printf("%lld ", x);
            printf("\n");
            return 1;
        }
    }
    printf("OK t2 %d cases\n", cases);
    // 题2 固定小用例
    {
        vector<ll> n1 = {3, 2, 5, 1, 4};
        printf("例 [3,2,5,1,4]: opt=%lld brute=%lld\n", maxBalancedSubsequenceSum(n1), brute2(n1));
        vector<ll> n2 = {-1, -2, -3};
        printf("例 [-1,-2,-3]: opt=%lld brute=%lld\n", maxBalancedSubsequenceSum(n2), brute2(n2));
        vector<ll> n3 = {5, -5, 5, -5};
        printf("例 [5,-5,5,-5]: opt=%lld brute=%lld\n", maxBalancedSubsequenceSum(n3), brute2(n3));
        vector<ll> n4 = {0, 3, 1, 6, 2};
        printf("例 [0,3,1,6,2]: opt=%lld brute=%lld\n", maxBalancedSubsequenceSum(n4), brute2(n4));
    }
    // 题4 对拍
    cases = 4000;
    for (int tc = 1; tc <= cases; tc++) {
        int n = rng() % 12 + 1;
        int k = rng() % 26;
        string s;
        for (int i = 0; i < n; i++) s += (char)('a' + rng() % 26);
        int a = longestIdealString(s, k);
        int b = brute4(s, k);
        if (a != b) {
            printf("MISMATCH t4 tc=%d s=%s k=%d opt=%d brute=%d\n", tc, s.c_str(), k, a, b);
            return 1;
        }
    }
    printf("OK t4 %d cases\n", cases);
    // 题4 固定小用例
    {
        printf("例 acfgbd k=2: opt=%d brute=%d\n", longestIdealString("acfgbd", 2), brute4("acfgbd", 2));
        printf("例 abcd k=1: opt=%d brute=%d\n", longestIdealString("abcd", 1), brute4("abcd", 1));
        printf("例 zyxzyx k=0: opt=%d brute=%d\n", longestIdealString("zyxzyx", 0), brute4("zyxzyx", 0));
        printf("例 pvpt k=2: opt=%d brute=%d\n", longestIdealString("pvpt", 2), brute4("pvpt", 2));
    }
    return 0;
}
