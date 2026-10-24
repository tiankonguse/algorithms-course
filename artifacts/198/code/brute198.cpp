#include <bits/stdc++.h>
using namespace std;

// 题5 划分可重集暴力：枚举 k 0..maxV，检查 2^n 二值划分（真=a 假=b）
void p5() {
    int n, m; cin >> n >> m;
    vector<int> arr(n);
    int maxV = 0;
    for (int i = 0; i < n; i++) { cin >> arr[i]; maxV = max(maxV, arr[i]); }
    vector<int> X(m), Y(m);
    for (int i = 0; i < m; i++) cin >> X[i] >> Y[i];
    auto ok = [&](int k) {
        for (int mask = 0; mask < (1 << n); mask++) {
            bool good = true;
            for (int i = 0; i < m && good; i++) {
                int a = X[i]-1, b = Y[i]-1;
                bool ba = (mask >> a) & 1, bb = (mask >> b) & 1;
                if (ba == bb) good = false; // 同进 a 或同进 b 都禁止
            }
            for (int i = 0; i < n && good; i++) {
                bool ina = (mask >> i) & 1;
                if (ina) {
                    for (int j = 0; j < i && good; j++)
                        if (arr[j] <= arr[i] - k && ((mask >> j) & 1)) good = false;
                } else {
                    for (int j = 0; j < i && good; j++)
                        if (arr[j] >= arr[i] + k && !((mask >> j) & 1)) good = false;
                }
            }
            if (good) return true;
        }
        return false;
    };
    int ans = -1;
    for (int k = 0; k <= maxV; k++) {
        if (ok(k)) { ans = k; break; }
    }
    cout << ans << "\n";
}

// 题6 喵了个喵 II 暴力：枚举 2^v 摆法，两序列完全相等
void p6() {
    int v; cin >> v;
    int n = v << 2;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    // 每个值 4 次出现的位置
    vector<vector<int>> pos(v + 1);
    for (int i = 0; i < n; i++) pos[a[i]].push_back(i);
    for (int mask = 0; mask < (1 << v); mask++) {
        vector<int> s0, s1;
        for (int i = 0; i < n; i++) {
            int val = a[i];
            // 该值在 pos 中的次序
            int rank = 0;
            while (pos[val][rank] != i) rank++;
            bool pat0011 = (mask >> (val - 1)) & 1;
            int seq; // 0 或 1
            if (pat0011) seq = (rank < 2) ? 0 : 1;
            else seq = (rank % 2 == 0) ? 0 : 1;
            if (seq == 0) s0.push_back(val); else s1.push_back(val);
        }
        if (s0.size() == s1.size() && s0 == s1) {
            cout << "Yes" << "\n";
            vector<int> pat(v + 1);
            for (int val = 1; val <= v; val++) pat[val] = (mask >> (val - 1)) & 1;
            for (int i = 0; i < n; i++) {
                int val = a[i];
                int rank = 0;
                while (pos[val][rank] != i) rank++;
                if (pat[val]) cout << (rank < 2 ? 0 : 1);
                else cout << (rank % 2 == 0 ? 0 : 1);
            }
            cout << "\n";
            return;
        }
    }
    cout << "No" << "\n";
}

int main() {
    int prob; cin >> prob;
    if (prob == 5) p5(); else p6();
    return 0;
}
