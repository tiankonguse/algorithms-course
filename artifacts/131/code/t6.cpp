// 题6 洛谷 P2605 基站选址：线段树最小值 + 链式前向星预警 vs 暴力
#include <bits/stdc++.h>
using namespace std;

/* ===== 课程实现（从 Code06 Java 改写） ===== */
const int MAXN6 = 20002;
int n6, k6;
int dist6[MAXN6], fix6[MAXN6], range6[MAXN6], warranty6[MAXN6];
int lft6[MAXN6], rgt6[MAXN6];
int head6[MAXN6], nxt6[MAXN6], to6[MAXN6];
int cnt6;
int segmin6[MAXN6 << 2], lazyadd6[MAXN6 << 2];
int dp6[MAXN6];

int search6(int d) {
    int l = 1, r = n6, m;
    int ans = 0;
    while (l <= r) {
        m = (l + r) / 2;
        if (dist6[m] >= d) { ans = m; r = m - 1; }
        else l = m + 1;
    }
    return ans;
}

void addEdge6(int u, int v) {
    nxt6[cnt6] = head6[u];
    to6[cnt6] = v;
    head6[u] = cnt6++;
}

void prepare6() {
    cnt6 = 1;
    for (int i = 1; i <= n6; i++) {
        lft6[i] = search6(dist6[i] - range6[i]);
        rgt6[i] = search6(dist6[i] + range6[i]);
        if (dist6[rgt6[i]] > dist6[i] + range6[i]) {
            rgt6[i]--;
        }
        addEdge6(rgt6[i], i);
    }
}

void up6(int i) { segmin6[i] = min(segmin6[i << 1], segmin6[i << 1 | 1]); }

void lazy6(int i, int v) { segmin6[i] += v; lazyadd6[i] += v; }

void down6(int i) {
    if (lazyadd6[i] != 0) {
        lazy6(i << 1, lazyadd6[i]);
        lazy6(i << 1 | 1, lazyadd6[i]);
        lazyadd6[i] = 0;
    }
}

void build6(int l, int r, int i) {
    if (l == r) {
        segmin6[i] = dp6[l];
    } else {
        int mid = (l + r) >> 1;
        build6(l, mid, i << 1);
        build6(mid + 1, r, i << 1 | 1);
        up6(i);
    }
    lazyadd6[i] = 0;
}

void add6(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        lazy6(i, jobv);
    } else {
        down6(i);
        int mid = (l + r) >> 1;
        if (jobl <= mid) add6(jobl, jobr, jobv, l, mid, i << 1);
        if (jobr > mid) add6(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        up6(i);
    }
}

int query6(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) return segmin6[i];
    down6(i);
    int mid = (l + r) >> 1;
    int ans = INT_MAX;
    if (jobl <= mid) ans = min(ans, query6(jobl, jobr, l, mid, i << 1));
    if (jobr > mid) ans = min(ans, query6(jobl, jobr, mid + 1, r, i << 1 | 1));
    return ans;
}

int compute6() {
    for (int i = 1, w = 0; i <= n6; i++) {
        dp6[i] = w + fix6[i];
        for (int ei = head6[i]; ei != 0; ei = nxt6[ei]) {
            w += warranty6[to6[ei]];
        }
    }
    for (int t = 2; t <= k6 + 1; t++) {
        build6(1, n6, 1);
        for (int i = 1; i <= n6; i++) {
            if (i >= t) {
                dp6[i] = min(dp6[i], query6(1, i - 1, 1, n6, 1) + fix6[i]);
            }
            for (int ei = head6[i], uncover; ei != 0; ei = nxt6[ei]) {
                uncover = to6[ei];
                if (lft6[uncover] > 1) {
                    add6(1, lft6[uncover] - 1, warranty6[uncover], 1, n6, 1);
                }
            }
        }
    }
    return dp6[n6];
}

void setup6(const vector<int>& dist, const vector<int>& fix, const vector<int>& range,
            const vector<int>& warranty, int n0, int K) {
    n6 = n0 + 1;
    k6 = K;
    memset(dist6, 0, sizeof(dist6));
    memset(fix6, 0, sizeof(fix6));
    memset(range6, 0, sizeof(range6));
    memset(warranty6, 0, sizeof(warranty6));
    for (int i = 1; i <= n0; i++) {
        dist6[i] = dist[i];
        fix6[i] = fix[i];
        range6[i] = range[i];
        warranty6[i] = warranty[i];
    }
    dist6[n6] = INT_MAX;
    fix6[n6] = range6[n6] = warranty6[n6] = 0;
    memset(head6, 0, sizeof(head6));
    memset(dp6, 0, sizeof(dp6));
    prepare6();
}

/* ===== 暴力 ===== */
int brute6(const vector<int>& dist, const vector<int>& fix, const vector<int>& range,
           const vector<int>& warranty, int n0, int K) {
    int N = n0 + 1;
    vector<int> d(N + 1), fx(N + 1), rg(N + 1), wr(N + 1), lf(N + 1), rt(N + 1);
    for (int i = 1; i <= n0; i++) { d[i] = dist[i]; fx[i] = fix[i]; rg[i] = range[i]; wr[i] = warranty[i]; }
    d[N] = INT_MAX; fx[N] = rg[N] = wr[N] = 0;
    auto search = [&](int x) {
        int l = 1, r = N, ans = 0;
        while (l <= r) { int m = (l + r) / 2; if (d[m] >= x) { ans = m; r = m - 1; } else l = m + 1; }
        return ans;
    };
    for (int i = 1; i <= N; i++) {
        lf[i] = search(d[i] - rg[i]);
        rt[i] = search(d[i] + rg[i]);
        if (rt[i] >= 1 && d[rt[i]] > d[i] + rg[i]) rt[i]--;
    }
    auto serviced = [&](int j, int p) { return lf[j] <= p && p <= rt[j]; };
    const int INF = 1e9;
    vector<vector<int>> dp(K + 2, vector<int>(N + 1, INF));
    for (int i = 1; i <= N; i++) {
        int c = 0;
        for (int j = 1; j < i; j++) if (!serviced(j, i)) c += wr[j];
        dp[1][i] = fx[i] + c;
    }
    for (int t = 2; t <= K + 1; t++) {
        for (int i = 1; i <= N; i++) {
            dp[t][i] = dp[t - 1][i];  // 最多 t-1 个基站的方案仍合法（Java dp 就地 min 的语义）
            if (i < t) continue;
            int best = INF;
            for (int p = 1; p <= i - 1; p++) {
                int c = 0;
                for (int j = p + 1; j <= i - 1; j++) {
                    if (!serviced(j, p) && !serviced(j, i)) c += wr[j];
                }
                best = min(best, dp[t - 1][p] + c);
            }
            dp[t][i] = min(dp[t][i], fx[i] + best);
        }
    }
    return dp[K + 1][N];
}

int main() {
    mt19937 rng(20261016);
    int cases = 400;
    for (int tc = 1; tc <= cases; tc++) {
        int n0 = rng() % 8 + 1;
        int K = rng() % min(4, n0) + 1;
        vector<int> dist(n0 + 1), fix(n0 + 1), range(n0 + 1), warranty(n0 + 1);
        int pos = 0;
        for (int i = 1; i <= n0; i++) {
            pos += (int)(rng() % 10) + 1;
            dist[i] = pos;
            fix[i] = (int)(rng() % 30) + 1;
            range[i] = (int)(rng() % 60) + 1;
            warranty[i] = (int)(rng() % 30) + 1;
        }
        setup6(dist, fix, range, warranty, n0, K);
        int opt = compute6();
        int b = brute6(dist, fix, range, warranty, n0, K);
        if (opt != b) {
            printf("MISMATCH tc=%d n=%d K=%d opt=%d brute=%d\n", tc, n0, K, opt, b);
            printf("dist: "); for (int i = 1; i <= n0; i++) printf("%d ", dist[i]); printf("\n");
            printf("fix: "); for (int i = 1; i <= n0; i++) printf("%d ", fix[i]); printf("\n");
            printf("range: "); for (int i = 1; i <= n0; i++) printf("%d ", range[i]); printf("\n");
            printf("wr: "); for (int i = 1; i <= n0; i++) printf("%d ", warranty[i]); printf("\n");
            return 1;
        }
    }
    printf("OK %d cases\n", cases);
    // 固定小用例
    {
        // 3 个村庄，dist=1,3,6；fix 各 1；range 足够大；warranty 各 5
        vector<int> dist = {0, 1, 3, 6};
        vector<int> fix = {0, 1, 1, 1};
        vector<int> range = {0, 100, 100, 100};
        vector<int> warranty = {0, 5, 5, 5};
        setup6(dist, fix, range, warranty, 3, 1);
        printf("例 n=3 大range k=1: opt=%d brute=%d\n", compute6(), brute6(dist, fix, range, warranty, 3, 1));
        // 全不覆盖：range=1
        vector<int> dist2 = {0, 2, 4, 6};
        vector<int> fix2 = {0, 3, 2, 1};
        vector<int> range2 = {0, 1, 1, 1};
        vector<int> warranty2 = {0, 10, 10, 10};
        setup6(dist2, fix2, range2, warranty2, 3, 2);
        printf("例 n=3 小range k=2: opt=%d brute=%d\n", compute6(), brute6(dist2, fix2, range2, warranty2, 3, 2));
    }
    return 0;
}
