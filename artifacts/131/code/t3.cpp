// 题3 洛谷 P3287 方伯伯的玉米田：二维树状数组（高度 × 修改次数）vs 暴力
#include <bits/stdc++.h>
using namespace std;

/* ===== 课程实现（从 Code03 Java 改写） ===== */
const int MAXH = 5500, MAXK = 501, MAXN = 10001;
int arr[MAXN];
int tree2d[MAXH + 1][MAXK + 1];
int n3, k3;

void update2d(int x, int y, int v) {
    for (int i = x; i <= MAXH; i += i & -i) {
        for (int j = y; j <= k3 + 1; j += j & -j) {
            tree2d[i][j] = max(tree2d[i][j], v);
        }
    }
}

int query2d(int x, int y) {
    int ans = 0;
    for (int i = x; i > 0; i -= i & -i) {
        for (int j = y; j > 0; j -= j & -j) {
            ans = max(ans, tree2d[i][j]);
        }
    }
    return ans;
}

int compute3() {
    int v, dp;
    for (int i = 1; i <= n3; i++) {
        for (int j = k3; j >= 0; j--) {
            v = arr[i] + j;
            dp = query2d(v, j + 1) + 1;
            update2d(v, j + 1, dp);
        }
    }
    return query2d(MAXH, k3 + 1);
}

void reset3() {
    memset(tree2d, 0, sizeof(tree2d));
}

/* ===== 暴力 ===== */
int brute3(const vector<int>& a, int K) {
    int n = a.size();
    // f[i][j] = 第 i 个玉米（1 起）恰好多加 j 次，以它结尾的最长不下降子序列
    // f[i][j] = 1 + max{ f[p][q] : p<i, q<=j, a[p]+q <= a[i]+j }
    vector<vector<int>> f(n, vector<int>(K + 1, 0));
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= K; j++) {
            f[i][j] = 1;
            for (int p = 0; p < i; p++) {
                for (int q = 0; q <= j; q++) {
                    if (a[p] + q <= a[i] + j) f[i][j] = max(f[i][j], f[p][q] + 1);
                }
            }
            ans = max(ans, f[i][j]);
        }
    }
    return ans;
}

int main() {
    mt19937 rng(20261016);
    int cases = 800;
    for (int tc = 1; tc <= cases; tc++) {
        int n = rng() % 10 + 1;
        int K = rng() % 6 + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 20) + 1;
        n3 = n; k3 = K;
        for (int i = 1; i <= n; i++) arr[i] = a[i - 1];
        reset3();
        int opt = compute3();
        int b = brute3(a, K);
        if (opt != b) {
            printf("MISMATCH tc=%d n=%d K=%d opt=%d brute=%d arr=", tc, n, K, opt, b);
            for (int x : a) printf("%d ", x);
            printf("\n");
            return 1;
        }
    }
    printf("OK %d cases\n", cases);
    // 固定小用例
    {
        vector<int> a1 = {1, 2, 3, 4, 5};
        n3 = 5; k3 = 2; for (int i = 1; i <= 5; i++) arr[i] = a1[i - 1];
        reset3();
        printf("例 [1,2,3,4,5] k=2: opt=%d brute=%d\n", compute3(), brute3(a1, 2));
        vector<int> a2 = {5, 4, 3, 2, 1};
        n3 = 5; k3 = 2; for (int i = 1; i <= 5; i++) arr[i] = a2[i - 1];
        reset3();
        printf("例 [5,4,3,2,1] k=2: opt=%d brute=%d\n", compute3(), brute3(a2, 2));
        vector<int> a3 = {3, 1, 4, 1, 5, 9};
        n3 = 6; k3 = 3; for (int i = 1; i <= 6; i++) arr[i] = a3[i - 1];
        reset3();
        printf("例 [3,1,4,1,5,9] k=3: opt=%d brute=%d\n", compute3(), brute3(a3, 3));
    }
    return 0;
}
