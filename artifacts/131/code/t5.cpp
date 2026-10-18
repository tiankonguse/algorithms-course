// 题5 CF833B 面包店：线段树区间加（0~n 范围）vs 暴力
#include <bits/stdc++.h>
using namespace std;

/* ===== 课程实现（从 Code05 Java 改写） ===== */
const int MAXN = 35001;
int n5, k5;
int arr5[MAXN];
int dp5[MAXN];
int pre5[MAXN];
int segmax5[MAXN << 2];
int lazyadd5[MAXN << 2];

void up5(int i) {
    segmax5[i] = max(segmax5[i << 1], segmax5[i << 1 | 1]);
}

void lazy5(int i, int v) {
    segmax5[i] += v;
    lazyadd5[i] += v;
}

void down5(int i) {
    if (lazyadd5[i] != 0) {
        lazy5(i << 1, lazyadd5[i]);
        lazy5(i << 1 | 1, lazyadd5[i]);
        lazyadd5[i] = 0;
    }
}

void build5(int l, int r, int i) {
    if (l == r) {
        segmax5[i] = dp5[l];
    } else {
        int mid = (l + r) >> 1;
        build5(l, mid, i << 1);
        build5(mid + 1, r, i << 1 | 1);
        up5(i);
    }
    lazyadd5[i] = 0;
}

void add5(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        lazy5(i, jobv);
    } else {
        down5(i);
        int mid = (l + r) >> 1;
        if (jobl <= mid) add5(jobl, jobr, jobv, l, mid, i << 1);
        if (jobr > mid) add5(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        up5(i);
    }
}

int query5(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) return segmax5[i];
    down5(i);
    int mid = (l + r) >> 1;
    int ans = INT_MIN;
    if (jobl <= mid) ans = max(ans, query5(jobl, jobr, l, mid, i << 1));
    if (jobr > mid) ans = max(ans, query5(jobl, jobr, mid + 1, r, i << 1 | 1));
    return ans;
}

int compute5() {
    memset(dp5, 0, sizeof(dp5));  // dp[0..n]
    for (int t = 1; t <= k5; t++) {
        build5(0, n5, 1);
        memset(pre5, 0, sizeof(pre5));
        for (int i = 1; i <= n5; i++) {
            add5(pre5[arr5[i]], i - 1, 1, 0, n5, 1);
            if (i >= t) {
                dp5[i] = query5(0, i - 1, 0, n5, 1);
            }
            pre5[arr5[i]] = i;
        }
    }
    return dp5[n5];
}

/* ===== 暴力 ===== */
// dp[t][i] = max over p in [t-1, i-1] (dp[t-1][p] + distinct(p+1, i))
int brute5(const vector<int>& a, int K) {
    int n = a.size() - 1;  // a[0] 是哨兵，实际数据 a[1..n]
    vector<vector<int>> dp(K + 1, vector<int>(n + 1, 0));
    for (int t = 1; t <= K; t++) {
        for (int i = t; i <= n; i++) {
            int best = INT_MIN;
            for (int p = t - 1; p <= i - 1; p++) {
                set<int> st;
                for (int x = p + 1; x <= i; x++) st.insert(a[x]);
                best = max(best, dp[t - 1][p] + (int)st.size());
            }
            dp[t][i] = best;
        }
    }
    return dp[K][n];
}

int main() {
    mt19937 rng(20261016);
    int cases = 600;
    for (int tc = 1; tc <= cases; tc++) {
        int n = rng() % 9 + 1;
        int K = rng() % min(4, n) + 1;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = (int)(rng() % n) + 1;
        n5 = n; k5 = K;
        for (int i = 1; i <= n; i++) arr5[i] = a[i];
        int opt = compute5();
        int b = brute5(a, K);
        if (opt != b) {
            printf("MISMATCH tc=%d n=%d K=%d opt=%d brute=%d arr=", tc, n, K, opt, b);
            for (int i = 1; i <= n; i++) printf("%d ", a[i]);
            printf("\n");
            return 1;
        }
    }
    printf("OK %d cases\n", cases);
    // 固定小用例
    {
        vector<int> a1 = {0, 1, 2, 1, 3};  // 1 起：1,2,1,3
        n5 = 4; k5 = 2; for (int i = 1; i <= 4; i++) arr5[i] = a1[i];
        printf("例 [1,2,1,3] k=2: opt=%d brute=%d\n", compute5(), brute5(a1, 2));
        vector<int> a2 = {0, 1, 2, 3, 4};  // 1,2,3,4
        n5 = 4; k5 = 3; for (int i = 1; i <= 4; i++) arr5[i] = a2[i];
        printf("例 [1,2,3,4] k=3: opt=%d brute=%d\n", compute5(), brute5(a2, 3));
    }
    return 0;
}
