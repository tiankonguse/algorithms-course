#include <bits/stdc++.h>
using namespace std;

// 题1：休息k分钟最大会议和
// 课程法：jump 数组预处理（滑动窗口）+ 倒序 dp
// 暴力法：直接枚举每个 i 向后累加直到 >= k

long long best1(const vector<int>& arr, int k) {  // 暴力 O(n^2)
    int n = arr.size();
    vector<long long> dp(n + 1, 0);
    for (int i = n - 1; i >= 0; i--) {
        int j = i + 1, sum = 0;
        while (j < n && sum < k) sum += arr[j++];
        dp[i] = max(dp[i + 1], dp[j] + (long long)arr[i]);
    }
    return dp[0];
}

long long best2(const vector<int>& arr, int k) {  // 课程：jump 预处理 O(n)
    int n = arr.size();
    vector<int> jump(n, 0);
    for (int i = 0, l = 1, r = 1, sum = 0; i < n - 1; i++, l++) {
        while (r < n && sum < k) sum += arr[r++];
        jump[i] = r;
        sum -= arr[l];
    }
    jump[n - 1] = n;
    vector<long long> dp(n + 1, 0);
    for (int i = n - 1; i >= 0; i--) {
        dp[i] = max(dp[i + 1], dp[jump[i]] + (long long)arr[i]);
    }
    return dp[0];
}

int main() {
    // 固定例
    {
        vector<int> arr = {200, 5, 6, 14, 7, 300};
        int k = 15;
        printf("固定例: best1=%lld best2=%lld (期望 500)\n", best1(arr, k), best2(arr, k));
        assert(best1(arr, k) == 500 && best2(arr, k) == 500);
    }
    {
        vector<int> arr = {1, 1, 1};
        printf("固定例 [1,1,1] k=1: %lld %lld\n", best1(arr, 1), best2(arr, 1));
    }
    mt19937 rnd(20261016);
    int T = 20000, bad = 0;
    for (int t = 0; t < T; t++) {
        int n = rnd() % 12 + 1;
        int v = rnd() % 15 + 1;
        int k = rnd() % 20 + 1;
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = rnd() % v + 1;
        long long a = best1(arr, k), b = best2(arr, k);
        if (a != b) {
            bad++;
            printf("MISMATCH n=%d k=%d\n", n, k);
            for (int x : arr) printf("%d ", x);
            printf("\na=%lld b=%lld\n", a, b);
            if (bad > 5) break;
        }
    }
    printf("随机 %d 组, bad=%d\n", T, bad);
    return bad ? 1 : 0;
}
