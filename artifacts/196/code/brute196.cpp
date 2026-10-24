#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ===== 题1 2-SAT 模版 P4782 =====
// m 行: x1 v1 x2 v2 = (x1==v1) 或 (x2==v2)
static void p1() {
    int n, m;
    if (!(cin >> n >> m)) return;
    vector<pair<int,int>> cl;
    for (int i = 0; i < m; i++) {
        int x1, v1, x2, v2;
        cin >> x1 >> v1 >> x2 >> v2;
        cl.push_back({x1 * 2 + v1, x2 * 2 + v2}); // 点编号: x*2+v
    }
    // 枚举 2^n
    for (int mask = 0; mask < (1 << n); mask++) {
        bool ok = true;
        for (auto &c : cl) {
            int x1 = c.first / 2, v1 = c.first % 2;
            int x2 = c.second / 2, v2 = c.second % 2;
            bool b1 = ((mask >> (x1 - 1)) & 1) == v1;
            bool b2 = ((mask >> (x2 - 1)) & 1) == v2;
            if (!(b1 || b2)) { ok = false; break; }
        }
        if (ok) {
            cout << "POSSIBLE\n";
            for (int i = 1; i <= n; i++) cout << ((mask >> (i - 1)) & 1) << (i == n ? "\n" : " ");
            return;
        }
    }
    cout << "IMPOSSIBLE\n";
}

// ===== 题2 和平委员会 P5782 =====
// n 团体 2n 代表(团体i: 2i-1, 2i), m 讨厌对
static void p2() {
    int n, m;
    if (!(cin >> n >> m)) return;
    vector<pair<int,int>> bad;
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        bad.push_back({u, v});
    }
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> sel;
        for (int i = 1; i <= n; i++) sel.push_back(((mask >> (i - 1)) & 1) ? 2 * i : 2 * i - 1);
        bool ok = true;
        for (auto &b : bad) {
            if (binary_search(sel.begin(), sel.end(), b.first) && binary_search(sel.begin(), sel.end(), b.second)) {
                ok = false; break;
            }
        }
        if (ok) {
            for (int x : sel) cout << x << "\n";
            return;
        }
    }
    cout << "NIE\n";
}

// ===== 题3 宇航员 UVA1391 =====
// 多测 n m (0 0 结束)。每人 A/B/C；avg = sumAge/n 向上取整? 课程: age*n >= sumAge → age >= avg_real
static void p3() {
    while (true) {
        int n, m;
        cin >> n >> m;
        if (n == 0 && m == 0) break;
        vector<int> age(n + 1);
        ll sum = 0;
        for (int i = 1; i <= n; i++) { cin >> age[i]; sum += age[i]; }
        vector<pair<int,int>> bad;
        for (int i = 0; i < m; i++) {
            int x, y; cin >> x >> y;
            bad.push_back({x, y});
        }
        // 任务: 0=A 1=B 2=C
        bool found = false;
        vector<int> ans;
        // 能力: age*n>=sum → 可 A 或 C; 否则可 B 或 C
        for (long long code = 0; code < pow(3LL, n) && !found; code++) {
            long long c = code;
            vector<int> task(n + 1);
            for (int i = 1; i <= n; i++) { task[i] = c % 3; c /= 3; }
            bool ok = true;
            for (int i = 1; i <= n && ok; i++) {
                bool older = age[i] * n >= sum;
                if (task[i] == 0 && !older) ok = false;
                if (task[i] == 1 && older) ok = false;
                if (task[i] == 2) ok = true;
            }
            for (auto &b : bad) {
                if (task[b.first] == task[b.second]) { ok = false; break; }
            }
            if (ok) { found = true; ans = task; }
        }
        if (!found) cout << "No solution.\n";
        else {
            for (int i = 1; i <= n; i++) cout << (char)('A' + ans[i]) << "\n";
        }
    }
}

// ===== 题4 约束 CF1697F =====
// 多测 t。n m k；非降数组值 1..k；约束 1 i x / 2 i j x / 3 i j x
static void p4() {
    int t;
    cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        struct C { int op, i, j, x; };
        vector<C> cs;
        for (int z = 0; z < m; z++) {
            int op; cin >> op;
            if (op == 1) { int i, x; cin >> i >> x; cs.push_back({1, i, 0, x}); }
            else { int i, j, x; cin >> i >> j >> x; cs.push_back({op, i, j, x}); }
        }
        // 枚举非降数组（值 1..k）
        bool found = false;
        vector<int> ans;
        // 组合枚举: n 个位置从 1..k 非降 => 用 DFS
        vector<int> cur(n + 1);
        function<bool(int, int)> dfs = [&](int pos, int lo) -> bool {
            if (pos > n) {
                for (auto &c : cs) {
                    if (c.op == 1) { if (cur[c.i] == c.x) return false; }
                    else if (c.op == 2) { if (cur[c.i] + cur[c.j] > c.x) return false; }
                    else { if (cur[c.i] + cur[c.j] < c.x) return false; }
                }
                ans = cur; return true;
            }
            for (int v = lo; v <= k; v++) { cur[pos] = v; if (dfs(pos + 1, v)) return true; }
            return false;
        };
        if (dfs(1, 1)) {
            for (int i = 1; i <= n; i++) cout << ans[i] << (i == n ? "\n" : " ");
        } else cout << -1 << "\n";
    }
}

// ===== 题5 同谋者 P3513 =====
// n 行认识列表；交流队内部全认识、保密队内部全不认识、两队非空；数方法数
static void p5() {
    int n;
    cin >> n;
    vector<vector<bool>> know(n + 1, vector<bool>(n + 1, false));
    for (int i = 1; i <= n; i++) {
        int k; cin >> k;
        for (int j = 0; j < k; j++) { int x; cin >> x; know[i][x] = true; }
    }
    ll cnt = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        // 位 1 = 交流队, 0 = 保密队
        vector<int> a, b;
        for (int i = 1; i <= n; i++) (((mask >> (i - 1)) & 1) ? a : b).push_back(i);
        if (a.empty() || b.empty()) continue;
        bool ok = true;
        for (size_t i = 0; i < a.size() && ok; i++)
            for (size_t j = i + 1; j < a.size() && ok; j++)
                if (!know[a[i]][a[j]]) ok = false;
        for (size_t i = 0; i < b.size() && ok; i++)
            for (size_t j = i + 1; j < b.size() && ok; j++)
                if (know[b[i]][b[j]]) ok = false;
        if (ok) cnt++;
    }
    cout << cnt << "\n";
}

// ===== 题6 谜语 P6378 =====
// n 点 m 边 k 阵营；每阵营 w 个点；关键点：每阵营≥1、每条边≥1端点
static void p6() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<pair<int,int>> edges;
    for (int i = 0; i < m; i++) { int u, v; cin >> u >> v; edges.push_back({u, v}); }
    vector<vector<int>> camps(k);
    for (int i = 0; i < k; i++) {
        int w; cin >> w;
        camps[i].resize(w);
        for (int j = 0; j < w; j++) cin >> camps[i][j];
    }
    for (int mask = 0; mask < (1 << n); mask++) {
        bool ok = true;
        // 每个阵营恰好一个关键点（官方题意）
        for (auto &c : camps) {
            int cntk = 0;
            for (int x : c) if ((mask >> (x - 1)) & 1) cntk++;
            if (cntk != 1) { ok = false; break; }
        }
        if (!ok) continue;
        for (auto &e : edges) {
            if (!(((mask >> (e.first - 1)) & 1) || ((mask >> (e.second - 1)) & 1))) { ok = false; break; }
        }
        if (ok) { cout << "TAK\n"; return; }
    }
    cout << "NIE\n";
}

// ===== 题7 互质 AT_abc210_f =====
// n 卡片 a b；选值后两两互质
static void p7() {
    int n;
    cin >> n;
    vector<pair<int,int>> cards(n + 1);
    for (int i = 1; i <= n; i++) cin >> cards[i].first >> cards[i].second;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> vals;
        for (int i = 1; i <= n; i++) vals.push_back((mask >> (i - 1)) & 1 ? cards[i].second : cards[i].first);
        bool ok = true;
        for (size_t i = 0; i < vals.size() && ok; i++)
            for (size_t j = i + 1; j < vals.size() && ok; j++)
                if (std::gcd(vals[i], vals[j]) > 1) ok = false;
        if (ok) { cout << "Yes\n"; return; }
    }
    cout << "No\n";
}

// ===== 题8 电台 CF1215F =====
// n 要求对 u v(至少签一个), p 电台(l r), f 频率, m 限制对(不同时签)
// 签约电台必须 [l,r] 含 ansx
static void p8() {
    int n, p, f, m;
    cin >> n >> p >> f >> m;
    vector<pair<int,int>> req;
    for (int i = 0; i < n; i++) { int u, v; cin >> u >> v; req.push_back({u, v}); }
    vector<pair<int,int>> lr(p + 1);
    for (int i = 1; i <= p; i++) cin >> lr[i].first >> lr[i].second;
    vector<pair<int,int>> ban;
    for (int i = 0; i < m; i++) { int u, v; cin >> u >> v; ban.push_back({u, v}); }
    for (int ax = 1; ax <= f; ax++) {
        for (int mask = 0; mask < (1 << p); mask++) {
            vector<int> pick;
            for (int i = 1; i <= p; i++) if ((mask >> (i - 1)) & 1) pick.push_back(i);
            bool ok = true;
            // 签约电台必须含 ax
            for (int i : pick) if (lr[i].first > ax || lr[i].second < ax) { ok = false; break; }
            if (!ok) continue;
            for (auto &r : req) {
                bool in1 = (mask >> (r.first - 1)) & 1, in2 = (mask >> (r.second - 1)) & 1;
                if (!(in1 || in2)) { ok = false; break; }
            }
            if (!ok) continue;
            for (auto &b : ban) {
                bool in1 = (mask >> (b.first - 1)) & 1, in2 = (mask >> (b.second - 1)) & 1;
                if (in1 && in2) { ok = false; break; }
            }
            if (ok) {
                cout << pick.size() << " " << ax << "\n";
                for (int i : pick) cout << i << " ";
                cout << "\n";
                return;
            }
        }
    }
    cout << -1 << "\n";
}

int main(int argc, char **argv) {
    int no = argc > 1 ? atoi(argv[1]) : 1;
    switch (no) {
        case 1: p1(); break; case 2: p2(); break; case 3: p3(); break;
        case 4: p4(); break; case 5: p5(); break; case 6: p6(); break;
        case 7: p7(); break; case 8: p8(); break;
    }
    return 0;
}
