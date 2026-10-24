#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1LL << 60;

// ---------- 题1 遗产：直接建真实边 + Bellman-Ford ----------
string solve1(istream& in) {
    int n, q, s; in >> n >> q >> s;
    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < q; i++) {
        int op; in >> op;
        if (op == 1) { int x, y, w; in >> x >> y >> w; adj[x].push_back({y, w}); }
        else if (op == 2) { int x, l, r, w; in >> x >> l >> r >> w; for (int v = l; v <= r; v++) adj[x].push_back({v, w}); }
        else { int x, l, r, w; in >> x >> l >> r >> w; for (int v = l; v <= r; v++) adj[v].push_back({x, w}); }
    }
    vector<ll> dist(n + 1, INF); dist[s] = 0;
    for (int k = 0; k < n; k++)
        for (int u = 1; u <= n; u++) if (dist[u] < INF)
            for (auto& e : adj[u]) if (dist[e.first] > dist[u] + e.second) dist[e.first] = dist[u] + e.second;
    string out;
    for (int i = 1; i <= n; i++) out += to_string(dist[i] == INF ? -1 : dist[i]) + " ";
    return out + "\n";
}

// ---------- 题2 旅程：展开区间连区间 + BFS ----------
string solve2(istream& in) {
    int n, m, p; in >> n >> m >> p;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c, d; in >> a >> b >> c >> d;
        for (int u = a; u <= b; u++)
            for (int v = c; v <= d; v++) { adj[u].push_back(v); adj[v].push_back(u); }
    }
    vector<int> dist(n + 1, -1); dist[p] = 0;
    queue<int> qu; qu.push(p);
    while (!qu.empty()) {
        int u = qu.front(); qu.pop();
        for (int v : adj[u]) if (dist[v] == -1) { dist[v] = dist[u] + 1; qu.push(v); }
    }
    string out;
    for (int i = 1; i <= n; i++) out += to_string(dist[i]) + "\n";
    return out;
}

// ---------- 题3 道路：展开边 + 分层 Bellman-Ford ----------
string solve3(istream& in) {
    int t; in >> t;
    int n, m, k; in >> n >> m >> k;
    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c, d, w; in >> a >> b >> c >> d >> w;
        for (int u = a; u <= b; u++)
            for (int v = c; v <= d; v++) { adj[u].push_back({v, w}); adj[v].push_back({u, w}); }
    }
    vector<vector<ll>> dist(n + 1, vector<ll>(k + 1, INF));
    dist[1][0] = 0;
    // 状态图 Bellman-Ford：状态 (u, used) 边：正常 (v,used) w；免费 (v,used+1) 0
    int S = n * (k + 1);
    for (int it = 0; it < S; it++) {
        bool chg = false;
        for (int u = 1; u <= n; u++)
            for (int used = 0; used <= k; used++) if (dist[u][used] < INF)
                for (auto& e : adj[u]) {
                    int v = e.first, w = e.second;
                    if (dist[v][used] > dist[u][used] + w) { dist[v][used] = dist[u][used] + w; chg = true; }
                    if (used < k && dist[v][used + 1] > dist[u][used]) { dist[v][used + 1] = dist[u][used]; chg = true; }
                }
        if (!chg) break;
    }
    ll best = INF;
    for (int used = 0; used <= k; used++) best = min(best, dist[n][used]);
    return (best == INF ? string("CreationAugust is a sb!\n") : to_string(best) + "\n");
}

// ---------- 题4 沙漠：差分约束 Bellman-Ford 判负环 ----------
string solve4(istream& in) {
    int n, s, m; in >> n >> s >> m;
    // 节点：1..n 变量；L 上限节点；O 下限节点；C1..Cs 定值节点
    int L = n + 1, O = n + 2;
    vector<int> val(n + 1, 0);
    for (int i = 0; i < s; i++) { int x, v; in >> x >> v; val[x] = v; }
    struct E { int u, v, w; };
    vector<E> edges;
    vector<ll> dist(n + 3, 1000000000LL);
    dist[L] = 1000000000LL;
    dist[O] = 1;
    // L -> x 0（x <= LIMIT）；x -> O 0（O <= x，即 x >= 1）
    for (int x = 1; x <= n; x++) { edges.push_back({L, x, 0}); edges.push_back({x, O, 0}); }
    // 定值：x -> Cx 0、Cx -> x 0，Cx 初始 v
    for (int x = 1; x <= n; x++) if (val[x]) {
        int C = n + 3 + (x - 1);
        dist.push_back(0); // 占位，改
    }
    // 重新规划：定值节点用向量
    dist.assign(n + 3 + n, 1000000000LL);
    for (int x = 1; x <= n; x++) if (val[x]) {
        int C = n + 3 + (x - 1);
        dist[C] = val[x];
        edges.push_back({x, C, 0});
        edges.push_back({C, x, 0});
    }
    for (int i = 0; i < m; i++) {
        int l, r, k; in >> l >> r >> k;
        int vnode = n + 3 + n + i;      // 每关系一个 vnode
        dist.push_back(1000000000LL);
        vector<int> xs(k);
        for (int j = 0; j < k; j++) { int x; in >> x; xs[j] = x; edges.push_back({x, vnode, 0}); }
        // vnode -> 区间其余点，权 -1
        for (int p = 1; p <= n; p++) {
            if (p >= l && p <= r && find(xs.begin(), xs.end(), p) == xs.end())
                edges.push_back({vnode, p, -1});
        }
    }
    int N = dist.size() - 1;
    // Bellman-Ford 判负环
    for (int it = 0; it <= N; it++) {
        bool chg = false;
        for (auto& e : edges) if (dist[e.u] < INF && dist[e.v] > dist[e.u] + e.w) { dist[e.v] = dist[e.u] + e.w; chg = true; }
        if (it == N && chg) return "NIE\n";
        if (!chg) break;
    }
    // 检查值域
    for (int x = 1; x <= n; x++) if (dist[x] < 1 || dist[x] > 1000000000LL) return "NIE\n";
    string out = "TAK\n";
    for (int x = 1; x <= n; x++) out += to_string(dist[x]) + " ";
    return out + "\n";
}

// ---------- 题5 炸弹：逐点 BFS 连锁 ----------
string solve5(istream& in) {
    int n; in >> n;
    vector<ll> loc(n + 1), r(n + 1);
    for (int i = 1; i <= n; i++) in >> loc[i] >> r[i];
    ll ans = 0;
    for (int i = 1; i <= n; i++) {
        vector<char> ok(n + 1, 0); ok[i] = 1;
        bool chg = true;
        while (chg) {
            chg = false;
            for (int j = 1; j <= n; j++) if (ok[j]) {
                ll L = loc[j] - r[j], R = loc[j] + r[j];
                for (int k = 1; k <= n; k++) if (!ok[k] && loc[k] >= L && loc[k] <= R) { ok[k] = 1; chg = true; }
            }
        }
        int q = 0;
        for (int j = 1; j <= n; j++) q += ok[j];
        ans += 1LL * i * q;
    }
    return to_string(ans % 1000000007) + "\n";
}

// ---------- 题6/7 美丽的树：枚举排列验证 ----------
// 输入：n m，n-1 边，m 行 op a b c
string solve67(istream& in, bool want) {
    (void)want;
    int n, m; in >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) { int u, v; in >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    struct R { int op, a, b, c; };
    vector<R> rels(m);
    for (int i = 0; i < m; i++) { int op, a, b, c; in >> op >> a >> b >> c; rels[i] = {op, a, b, c}; }
    // 树上路径
    auto path = [&](int a, int b) {
        // BFS parent
        vector<int> par(n + 1, 0), dep(n + 1, 0);
        queue<int> qu; qu.push(a); par[a] = -1;
        while (!qu.empty()) { int u = qu.front(); qu.pop(); for (int v : adj[u]) if (!par[v]) { par[v] = u; dep[v] = dep[u] + 1; qu.push(v); } }
        vector<int> p;
        int x = b;
        while (x != -1) { p.push_back(x); if (x == a) break; x = par[x]; }
        reverse(p.begin(), p.end());
        return p;
    };
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 1);
    do {
        bool ok = true;
        vector<int> val(n + 1);
        for (int i = 0; i < n; i++) val[perm[i]] = i + 1;
        for (auto& r : rels) {
            auto p = path(r.a, r.b);
            int mn = 1 << 30, mx = 0;
            for (int u : p) { mn = min(mn, val[u]); mx = max(mx, val[u]); }
            if (r.op == 1) { if (mn != val[r.c]) { ok = false; break; } }
            else { if (mx != val[r.c]) { ok = false; break; } }
        }
        if (ok) {
            string out;
            for (int i = 1; i <= n; i++) out += to_string(val[i]) + " ";
            return out + "\n";
        }
    } while (next_permutation(perm.begin(), perm.end()));
    return "-1\n";
}

// ---------- 题8 逛森林：顺序模拟 + 展开传送 + Dijkstra ----------
string solve8(istream& in) {
    int n, m, s; in >> n >> m >> s;
    struct Op1 { int a, b, c, d, w; };
    vector<Op1> ops1;
    vector<vector<pair<int,int>>> real(n + 1);   // op2 边
    // 读全部操作并顺序执行
    struct O { int op; int a, b, c, d, w; };
    vector<O> all(m);
    for (int i = 0; i < m; i++) {
        int op; in >> op;
        if (op == 1) { O o; cin >> o.a >> o.b >> o.c >> o.d >> o.w; o.op = 1; all[i] = o; }
        else { O o; cin >> o.a >> o.b >> o.w; o.op = 2; all[i] = o; }
    }
    auto conn = [&](int x, int y) {
        vector<char> vis(n + 1, 0); queue<int> qu; qu.push(x); vis[x] = 1;
        while (!qu.empty()) { int u = qu.front(); qu.pop(); if (u == y) return true; for (auto& e : real[u]) if (!vis[e.first]) { vis[e.first] = 1; qu.push(e.first); } }
        return x == y;
    };
    for (auto& o : all) {
        if (o.op == 2) {
            if (!conn(o.a, o.b)) { real[o.a].push_back({o.b, o.w}); real[o.b].push_back({o.a, o.w}); }
        } else {
            if (conn(o.a, o.b) && conn(o.c, o.d)) ops1.push_back({o.a, o.b, o.c, o.d, o.w});
        }
    }
    // 树上路径（在 real 森林上，树 = 无环连通）
    auto path = [&](int a, int b) {
        vector<int> par(n + 1, 0);
        queue<int> qu; qu.push(a); par[a] = -1;
        while (!qu.empty()) { int u = qu.front(); qu.pop(); for (auto& e : real[u]) if (!par[e.first]) { par[e.first] = u; qu.push(e.first); } }
        vector<int> p; int x = b;
        while (x != -1) { p.push_back(x); if (x == a) break; x = par[x]; }
        reverse(p.begin(), p.end());
        return p;
    };
    // 全图：real 边 + 传送边（路径->路径 单向，展开）
    vector<vector<pair<int,int>>> g(n + 1);
    for (int u = 1; u <= n; u++) for (auto& e : real[u]) if (u < e.first) { g[u].push_back(e); g[e.first].push_back({u, e.second}); }
    for (auto& o : ops1) {
        auto p1 = path(o.a, o.b), p2 = path(o.c, o.d);
        for (int u : p1) for (int v : p2) g[u].push_back({v, o.w});
    }
    vector<ll> dist(n + 1, INF); dist[s] = 0;
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq; pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto& e : g[u]) if (dist[e.first] > d + e.second) { dist[e.first] = d + e.second; pq.push({dist[e.first], e.first}); }
    }
    string out;
    for (int i = 1; i <= n; i++) out += to_string(dist[i] == INF ? -1 : dist[i]) + " ";
    return out + "\n";
}

// ---------- 题9 监狱：调度搜索（每囚犯沿唯一最短路只能前进） ----------
string solve9(istream& in) {
    int t; in >> t;
    string all;
    for (int ti = 0; ti < t; ti++) {
    int n; in >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) { int u, v; in >> u >> v; adj[u].push_back(v); adj[v].push_back(u); }
    int m; in >> m;
    vector<int> bed(m), work(m);
    for (int i = 0; i < m; i++) in >> bed[i] >> work[i];
    auto path = [&](int a, int b) {
        vector<int> par(n + 1, 0);
        queue<int> qu; qu.push(a); par[a] = -1;
        while (!qu.empty()) { int u = qu.front(); qu.pop(); for (int v : adj[u]) if (!par[v]) { par[v] = u; qu.push(v); } }
        vector<int> p; int x = b;
        while (x != -1) { p.push_back(x); if (x == a) break; x = par[x]; }
        reverse(p.begin(), p.end());
        return p;
    };
    vector<vector<int>> paths(m);
    for (int i = 0; i < m; i++) paths[i] = path(bed[i], work[i]);
    vector<int> step(m, 0);           // 已走步数
    // DFS：状态 (step, ...) → 是否全部到达
    map<vector<int>, bool> memo;
    function<bool(vector<int>&)> dfs = [&](vector<int>& st) {
        if (memo.count(st)) return memo[st];
        bool done = true;
        for (int i = 0; i < m; i++) if (st[i] < (int)paths[i].size() - 1) { done = false; break; }
        if (done) return memo[st] = true;
        // 当前占据位置
        for (int i = 0; i < m; i++) {
            if (st[i] >= (int)paths[i].size() - 1) continue;
            int nxt = paths[i][st[i] + 1];
            bool clash = false;
            for (int j = 0; j < m; j++) if (j != i) {
                int pos = paths[j][st[j]];
                if (pos == nxt) { clash = true; break; }
            }
            if (clash) continue;
            st[i]++;
            if (dfs(st)) { st[i]--; return memo[st] = true; }
            st[i]--;
        }
        return memo[st] = false;
    };
    vector<int> st0(m, 0);
    all += dfs(st0) ? "Yes\n" : "No\n";
    }
    return all;
}

int main(int argc, char** argv) {
    int tid = atoi(argv[1]);
    string out;
    switch (tid) {
        case 1: out = solve1(cin); break;
        case 2: out = solve2(cin); break;
        case 3: out = solve3(cin); break;
        case 4: out = solve4(cin); break;
        case 5: out = solve5(cin); break;
        case 6: out = solve67(cin, true); break;
        case 7: out = solve67(cin, true); break;
        case 8: out = solve8(cin); break;
        case 9: out = solve9(cin); break;
    }
    cout << out;
    return 0;
}
