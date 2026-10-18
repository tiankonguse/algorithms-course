// 题1 LC327 达标子数组个数：归并分治 + 树状数组离散化 vs 暴力
// 页面代码复现（dp-fenwick-range-sum.html）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/* ===== 课程实现：归并分治（页面代码） ===== */
const int MAXN = 100001;
ll sumv[MAXN], helpv[MAXN];
int lowv, upv;
int gn;  // 测试辅助：数组长度

int mergeCnt(int l, int m, int r) {
    int ans = 0;
    int wl = l, wr = l;
    ll mx, mn;
    for (int i = m + 1; i <= r; i++) {
        mx = sumv[i] - lowv;
        mn = sumv[i] - upv;
        while (wr <= m && sumv[wr] <= mx) wr++;
        while (wl <= m && sumv[wl] < mn) wl++;
        ans += wr - wl;
    }
    int p1 = l, p2 = m + 1, i = l;
    while (p1 <= m && p2 <= r) helpv[i++] = sumv[p1] <= sumv[p2] ? sumv[p1++] : sumv[p2++];
    while (p1 <= m) helpv[i++] = sumv[p1++];
    while (p2 <= r) helpv[i++] = sumv[p2++];
    for (i = l; i <= r; i++) sumv[i] = helpv[i];
    return ans;
}

int f(int l, int r) {
    if (l == r) {
        return lowv <= sumv[l] && sumv[l] <= upv ? 1 : 0;
    }
    int m = (l + r) / 2;
    return f(l, m) + f(m + 1, r) + mergeCnt(l, m, r);
}

int countRangeSumMerge(const vector<ll>& nums, int lower, int upper) {
    gn = nums.size();
    sumv[0] = nums[0];
    for (int i = 1; i < gn; i++) sumv[i] = sumv[i - 1] + nums[i];
    lowv = lower; upv = upper;
    return f(0, gn - 1);
}

/* ===== 课程实现：树状数组 + 离散化（页面代码） ===== */
ll sorted[MAXN];
int m;
int tree[MAXN];

int rankv(ll v) {
    int left = 1, right = m, mid;
    int ans = 0;
    while (left <= right) {
        mid = (left + right) / 2;
        if (sorted[mid] <= v) { ans = mid; left = mid + 1; }
        else right = mid - 1;
    }
    return ans;
}

void add(int i, int c) {
    while (i <= m) { tree[i] += c; i += i & -i; }
}

int presum(int i) {
    int ans = 0;
    while (i > 0) { ans += tree[i]; i -= i & -i; }
    return ans;
}

void buildBIT(const vector<ll>& nums) {
    int n = nums.size();
    for (int i = 1, j = 0; i <= n; i++, j++) sorted[i] = sorted[i - 1] + nums[j];
    sort(sorted + 1, sorted + n + 1);
    m = 1;
    for (int i = 2; i <= n; i++) if (sorted[m] != sorted[i]) sorted[++m] = sorted[i];
    for (int i = 1; i <= m; i++) tree[i] = 0;
}

int countRangeSumBIT(const vector<ll>& nums, int lower, int upper) {
    buildBIT(nums);
    ll s = 0;
    int ans = 0;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        s += nums[i];
        ans += presum(rankv(s - lower)) - presum(rankv(s - upper - 1));
        if (lower <= s && s <= upper) ans++;
        add(rankv(s), 1);
    }
    return ans;
}

/* ===== 暴力 ===== */
int brute(const vector<ll>& nums, int lower, int upper) {
    int n = nums.size(), ans = 0;
    for (int l = 0; l < n; l++) {
        ll s = 0;
        for (int r = l; r < n; r++) {
            s += nums[r];
            if (lower <= s && s <= upper) ans++;
        }
    }
    return ans;
}

int main() {
    mt19937 rng(20261016);
    int cases = 3000;
    for (int tc = 1; tc <= cases; tc++) {
        int n = rng() % 14 + 1;
        int lower = (int)(rng() % 41) - 20;
        int upper = lower + (int)(rng() % 21);
        vector<ll> nums(n);
        for (int i = 0; i < n; i++) nums[i] = (ll)(rng() % 41) - 20;
        int a = countRangeSumMerge(nums, lower, upper);
        int b = countRangeSumBIT(nums, lower, upper);
        int c = brute(nums, lower, upper);
        if (a != c || b != c) {
            printf("MISMATCH tc=%d n=%d lower=%d upper=%d merge=%d bit=%d brute=%d\n", tc, n, lower, upper, a, b, c);
            for (ll x : nums) printf("%lld ", x);
            printf("\n");
            return 1;
        }
    }
    printf("OK 3000 cases (merge==bit==brute)\n");
    // 固定小用例
    {
        vector<ll> n1 = {-2, 5, -1, 3};
        printf("例1 [-2,5,-1,3] l=-2 u=2: merge=%d bit=%d brute=%d\n",
               countRangeSumMerge(n1, -2, 2), countRangeSumBIT(n1, -2, 2), brute(n1, -2, 2));
        vector<ll> n2 = {1, 1, 1};
        printf("例2 [1,1,1] l=1 u=2: merge=%d bit=%d brute=%d\n",
               countRangeSumMerge(n2, 1, 2), countRangeSumBIT(n2, 1, 2), brute(n2, 1, 2));
    }
    return 0;
}
