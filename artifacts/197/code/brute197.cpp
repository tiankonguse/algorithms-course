#include <bits/stdc++.h>
using namespace std;

// 题目1 黑手党：暴力枚举摧毁子集
bool ok1(int n, int m, vector<int>& X, vector<int>& Y, vector<int>& C, int limit, vector<int>& T) {
    // 摧毁边只能取 t <= limit；t > limit 的边强制幸存
    for (int mask = 0; mask < (1 << m); mask++) {
        bool ok = true;
        vector<int> deg(n + 1, 0);
        for (int i = 0; i < m && ok; i++) if ((mask >> i) & 1) {
            if (T[i] > limit) ok = false;
            if (ok && (deg[X[i]] || deg[Y[i]])) ok = false;
            if (ok) { deg[X[i]]++; deg[Y[i]]++; }
        }
        if (!ok) continue;
        vector<map<int,int>> use(n + 1);
        for (int i = 0; i < m && ok; i++) if (!((mask >> i) & 1)) {
            if (use[X[i]].count(C[i]) || use[Y[i]].count(C[i])) ok = false;
            if (ok) { use[X[i]][C[i]] = 1; use[Y[i]][C[i]] = 1; }
        }
        if (ok) return true;
    }
    return false;
}

void p1() {
    int n, m; cin >> n >> m;
    vector<int> X(m), Y(m), C(m), T(m);
    int maxt = 0;
    for (int i = 0; i < m; i++) { cin >> X[i] >> Y[i] >> C[i] >> T[i]; maxt = max(maxt, T[i]); }
    // 与课程二分同语义：limit 从 1 起找最小可行
    int l = 1, r = maxt, best = -1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (ok1(n, m, X, Y, C, mid, T)) { best = mid; r = mid - 1; }
        else l = mid + 1;
    }
    if (best == -1) cout << "No\n";
    else cout << "Yes\n" << best << "\n";
}

// 题目2 旗帜：枚举全部旗子选择，检查不同旗子位置距离
void p2() {
    int n; cin >> n;
    vector<int> A(n), B(n);
    int mx = 0;
    for (int i = 0; i < n; i++) { cin >> A[i] >> B[i]; mx = max(mx, max(A[i], B[i])); }
    int ans = 0;
    for (int d = 0; d <= mx; d++) {
        bool ok = false;
        for (int mask = 0; mask < (1 << n); mask++) {
            vector<int> pos(n);
            for (int i = 0; i < n; i++) pos[i] = ((mask >> i) & 1) ? A[i] : B[i];
            bool good = true;
            for (int i = 0; i < n && good; i++)
                for (int j = i + 1; j < n && good; j++)
                    if (abs(pos[i] - pos[j]) < d) good = false;
            if (good) { ok = true; break; }
        }
        if (ok) ans = d; else break;
    }
    cout << ans << "\n";
}

// 题目3 编码：枚举 ? 取值，检查互不为前缀
void p3() {
    int n; cin >> n;
    vector<string> s(n);
    vector<int> qm(n, -1);
    for (int i = 0; i < n; i++) {
        cin >> s[i];
        for (int j = 0; j < (int)s[i].size(); j++) if (s[i][j] == '?') qm[i] = j;
    }
    bool found = false;
    int limit = 1 << n;
    for (int mask = 0; mask < limit && !found; mask++) {
        vector<string> t(n);
        bool good = true;
        for (int i = 0; i < n; i++) {
            t[i] = s[i];
            if (qm[i] != -1) t[i][qm[i]] = ((mask >> i) & 1) ? '1' : '0';
        }
        for (int i = 0; i < n && good; i++)
            for (int j = 0; j < n && good; j++) {
                if (i == j) continue;
                if (t[i].size() >= t[j].size()) {
                    if (t[i].substr(0, t[j].size()) == t[j]) good = false; // t[j] 是 t[i] 前缀
                }
            }
        if (good) found = true;
    }
    cout << (found ? "YES" : "NO") << "\n";
}

// 题目4 游戏：枚举全部地面取值，检查限制
void p4() {
    int n, d; cin >> n >> d;
    string g; cin >> g;
    vector<int> xpos;
    for (int i = 0; i < n; i++) if (g[i] == 'x') xpos.push_back(i);
    int m; cin >> m;
    vector<int> p1(m), p2(m);
    vector<char> c1(m), c2(m);
    for (int i = 0; i < m; i++) cin >> p1[i] >> c1[i] >> p2[i] >> c2[i];
    bool found = false;
    int t3 = 1, t2 = 1;
    for (int i = 0; i < d; i++) t3 *= 3;
    for (int i = 0; i < n - d; i++) t2 *= 2;
    for (int xv = 0; xv < t3 && !found; xv++) {
        for (int yv = 0; yv < t2 && !found; yv++) {
        int vx = xv, vy = yv;
        vector<char> car(n);
        for (int i = 0; i < n; i++) {
            if (g[i] == 'x') { car[i] = 'A' + (vx % 3); vx /= 3; }
            else if (g[i] == 'a') { car[i] = (vy & 1) ? 'B' : 'C'; vy >>= 1; }
            else if (g[i] == 'b') { car[i] = (vy & 1) ? 'A' : 'C'; vy >>= 1; }
            else { car[i] = (vy & 1) ? 'A' : 'B'; vy >>= 1; }
        }
        bool good = true;
        for (int i = 0; i < m && good; i++)
            if (car[p1[i] - 1] == c1[i] && car[p2[i] - 1] != c2[i]) good = false;
        if (good) found = true;
        }
    }
    cout << (found ? "YES" : "-1") << "\n";
}

// 题目5 布尔：暴力 first + 贪心
struct DSU5 {
    vector<int> fa;
    DSU5(int nn) : fa(nn + 1) { for (int i = 1; i <= nn; i++) fa[i] = i; }
    int find(int x) { while (x != fa[x]) x = fa[x]; return x; }
    void uni(int x, int y) {
        int fx = find(x), fy = find(y);
        if (fx != fy) fa[fx] = fy;
    }
};

void p5() {
    int n, m, q; cin >> n >> m >> q;
    vector<int> U(m), V(m), er(m + 1, 0);
    for (int i = 0; i < m; i++) {
        int x, y; cin >> U[i] >> x >> V[i] >> y;
        U[i] = x == 0 ? U[i] : U[i] + n;
        V[i] = y == 0 ? V[i] : V[i] + n;
        // 天生矛盾：u==v 且 x!=y -> U,V 互为反面
        if ((U[i] == V[i] + n || U[i] + n == V[i])) er[i + 1] = 1;
    }
    for (int i = 1; i <= m; i++) er[i] += er[i - 1];
    // first[i]：从 i 起第一条冲突限制位置（1..m+1）
    vector<int> first(m + 2);
    for (int i = 1; i <= m; i++) {
        DSU5 ds(2 * n);
        int j;
        for (j = i; j <= m; j++) {
            ds.uni(U[j - 1], V[j - 1]);
            ds.uni(U[j - 1] <= n ? U[j - 1] + n : U[j - 1] - n, V[j - 1] <= n ? V[j - 1] + n : V[j - 1] - n);
            if (ds.find(U[j - 1]) == (U[j - 1] <= n ? ds.find(U[j - 1] + n) : ds.find(U[j - 1] - n))) break;
        }
        first[i] = j > m ? m + 1 : j;
    }
    for (int qi = 0; qi < q; qi++) {
        int l, r; cin >> l >> r;
        if (er[r] - er[l - 1] > 0) { cout << -1 << "\n"; continue; }
        int cur = l, ans = 0;
        while (cur <= r) {
            ans++;
            if (first[cur] <= r) cur = first[cur];
            else cur = r + 1;
        }
        cout << ans << "\n";
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int prob = atoi(argv[1]);
    if (prob == 1) p1();
    else if (prob == 2) p2();
    else if (prob == 3) p3();
    else if (prob == 4) p4();
    else p5();
    return 0;
}
