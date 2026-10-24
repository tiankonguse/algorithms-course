// 194 课对拍暴力：./brute194.run <题号> < input
#include <bits/stdc++.h>
using namespace std;

// 邻接表 + 判连通
struct G {
    int n;
    vector<vector<pair<int,int>>> adj; // (to, edgeId)
    G(int n_ = 0): n(n_), adj(n_ + 1) {}
};

static vector<int> delV;   // 删除的点（题1/2/3/7/9/10 用）
static vector<int> delE;   // 删除的边 id（题6/7 用）

bool connected(const G& g, const vector<int>& verts) {
    // 从 verts 里第一个还在的点 BFS，数能访问到的 present 点个数；verts 空返回 true
    vector<int> present;
    vector<char> pm(g.n + 1, 0);
    for (int v : verts) if (!delV[v]) { present.push_back(v); pm[v] = 1; }
    if (present.empty()) return true;
    vector<char> vis(g.n + 1, 0);
    queue<int> qu;
    qu.push(present[0]);
    vis[present[0]] = 1;
    int seen = 1; // present[0] 自己
    while (!qu.empty()) {
        int u = qu.front(); qu.pop();
        for (auto& p : g.adj[u]) {
            int v = p.first, eid = p.second;
            if (delV[u] || delV[v]) continue;         // 点已删
            if (delE[eid]) continue;                  // 边已删
            if (!vis[v]) {
                vis[v] = 1;
                if (pm[v]) seen++;                    // 只数 present 点
                qu.push(v);
            }
        }
    }
    return seen == (int)present.size();
}

// x 与 y 是否连通（x==y 且 x 未删则连通；删点/删边由 delV/delE 决定）
bool connAB(const G& g, int x, int y) {
    if (delV[x] || delV[y]) return false;
    if (x == y) return true;
    vector<char> vis(g.n + 1, 0);
    queue<int> qu;
    qu.push(x); vis[x] = 1;
    while (!qu.empty()) {
        int u = qu.front(); qu.pop();
        for (auto& p : g.adj[u]) {
            int v = p.first, eid = p.second;
            if (delV[v] || delE[eid]) continue;
            if (!vis[v]) { vis[v] = 1; if (v == y) return true; qu.push(v); }
        }
    }
    return false;
}

// 题1：必经点计数。x,y 必经 v == x/y 或删 v 后 x,y 断连
string solve1(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    int q; in >> q;
    string out;
    while (q--) {
        int x, y; in >> x >> y;
        long long ans = 0;
        for (int v = 1; v <= n; v++) {
            if (v == x || v == y) { ans++; continue; }
            delV.assign(n + 1, 0);
            delE.assign(m + 1, 0);
            delV[v] = 1;
            if (!connAB(g, x, y)) ans++;
        }
        out += to_string(ans) + "\n";
    }
    return out;
}

// 题2：删点 z 后 x,y 是否连通（yes/no 小写）
string solve2(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    int q; in >> q;
    string out;
    while (q--) {
        int x, y, z; in >> x >> y >> z;
        if (x == z || y == z) { out += "yes\n"; continue; }
        delV.assign(n + 1, 0);
        delE.assign(m + 1, 0);
        delV[z] = 1;
        out += string(connAB(g, x, y) ? "no\n" : "yes\n"); // yes=断连
    }
    return out;
}

// 题3：a->b->c 无重复节点路径存在性（DFS 状压）
string solve3(istream& in) {
    int n, m, a, b, c;
    in >> n >> m >> a >> b >> c;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    // 枚举所有 a->...->b 简单路径，标记可达的「经过节点集合」；再检查从 b 出发不重复经过这些节点能否到 c
    vector<vector<int>> paths;
    vector<int> cur;
    vector<char> vis(n + 1, 0);
    function<void(int)> dfsA = [&](int u) {
        if (u == b) { paths.push_back(cur); return; }
        for (auto& p : g.adj[u]) {
            int v = p.first;
            if (v == b) { paths.push_back(cur); continue; } // 直接到 b
            if (!vis[v]) {
                vis[v] = 1; cur.push_back(v);
                dfsA(v);
                cur.pop_back(); vis[v] = 0;
            }
        }
    };
    vis[a] = 1; cur.push_back(a);
    dfsA(a);
    bool ok = false;
    for (auto& pa : paths) {
        vector<char> used(n + 1, 0);
        for (int v : pa) used[v] = 1;
        used[b] = 1;
        // b -> c 不重复
        function<bool(int)> dfsB = [&](int u) -> bool {
            if (u == c) return true;
            for (auto& p : g.adj[u]) {
                int v = p.first;
                if (!used[v]) { used[v] = 1; if (dfsB(v)) return true; used[v] = 0; }
            }
            return false;
        };
        if (dfsB(b)) { ok = true; break; }
    }
    return string(ok ? "Yes\n" : "No\n");
}

// 题4：城市阻断。删 i 邻边，数有序不连通对
string solve4(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    string out;
    for (int i = 1; i <= n; i++) {
        delV.assign(n + 1, 0);
        delE.assign(m + 1, 0);
        // 删 i 的所有邻边
        for (auto& p : g.adj[i]) delE[p.second] = 1;
        // 分块（i 本身保留）
        vector<char> vis(n + 1, 0);
        long long ans = 0;
        for (int s = 1; s <= n; s++) {
            if (vis[s] || delV[s]) continue;
            int sz = 0;
            queue<int> qu; qu.push(s); vis[s] = 1; sz++;
            while (!qu.empty()) {
                int u = qu.front(); qu.pop();
                for (auto& p : g.adj[u]) {
                    int v = p.first, eid = p.second;
                    if (delE[eid] || vis[v]) continue;
                    vis[v] = 1; sz++; qu.push(v);
                }
            }
            ans += 1LL * sz * (n - sz);
        }
        out += to_string(ans) + "\n";
    }
    return out;
}

// 题5：铁人两项。枚举三元组 (a,b,c) 检查 a->b->c 无重复路径
string solve5(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    long long ans = 0;
    for (int a = 1; a <= n; a++)
        for (int b = 1; b <= n; b++) {
            if (a == b) continue;
            for (int c = 1; c <= n; c++) {
                if (c == a || c == b) continue;
                // 检查 a->b->c 无重复点路径：DFS 状压（reached 标记是否已到过 b）
                vector<char> used(n + 1, 0);
                used[a] = 1;
                function<bool(int, bool)> go = [&](int u, bool reached) -> bool {
                    if (u == c) return reached;            // 未到过 b 就提前到 c：无效
                    if (u == b) {
                        for (auto& p : g.adj[u]) {
                            int v = p.first;
                            if (!used[v]) { used[v] = 1; if (go(v, true)) return true; used[v] = 0; }
                        }
                        return false;
                    }
                    for (auto& p : g.adj[u]) {
                        int v = p.first;
                        if (v == b) {
                            used[b] = 1;
                            if (go(b, true)) return true;
                            used[b] = 0;
                        } else if (!used[v]) {
                            used[v] = 1;
                            if (go(v, reached)) return true;
                            used[v] = 0;
                        }
                    }
                    return false;
                };
                if (go(a, false)) ans++;
            }
        }
    return to_string(ans) + "\n";
}

// 题6：边的查询。收集 x->y 所有简单路径上的边，删边后 x,y 仍可达则计数
string solve6(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    int q; in >> q;
    string out;
    while (q--) {
        int x, y; in >> x >> y;
        // 收集所有简单路径经过的边集
        vector<char> used(n + 1, 0);
        vector<int> eused(m + 1, 0);
        vector<char> onPath(m + 1, 0);
        function<void(int)> dfs = [&](int u) {
            if (u == y) return;
            for (auto& p : g.adj[u]) {
                int v = p.first, eid = p.second;
                if (!used[v] && !eused[eid]) {
                    used[v] = 1; eused[eid] = 1;
                    onPath[eid] = 1;
                    dfs(v);
                    used[v] = 0; eused[eid] = 0;
                }
            }
        };
        used[x] = 1;
        dfs(x);
        long long ans = 0;
        for (int e = 1; e <= m; e++) {
            if (!onPath[e]) continue;
            delV.assign(n + 1, 0);
            delE.assign(m + 1, 0);
            delE[e] = 1;
            vector<int> verts(n);
            iota(verts.begin(), verts.end(), 1);
            if (connected(g, verts)) ans++;
        }
        out += to_string(ans) + "\n";
    }
    return out;
}

// 题7：警察。op1 删边 cd（保证 cd 有边）；op2 删点 c
string solve7(istream& in) {
    int n, m;
    in >> n >> m;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    int q; in >> q;
    string out;
    while (q--) {
        int op; in >> op;
        if (op == 1) {
            int a, b, c, d; in >> a >> b >> c >> d;
            // 找到边 c-d 的 id（题面保证存在）
            int eid = -1;
            for (auto& p : g.adj[c]) if (p.first == d) { eid = p.second; break; }
            delV.assign(n + 1, 0);
            delE.assign(m + 1, 0);
            delE[eid] = 1;
            out += string(connAB(g, a, b) ? "yes\n" : "no\n");
        } else {
            int a, b, c; in >> a >> b >> c;
            delV.assign(n + 1, 0);
            delE.assign(m + 1, 0);
            delV[c] = 1;
            out += string(connAB(g, a, b) ? "yes\n" : "no\n");
        }
    }
    return out;
}

// 题8：游客。A x y = 所有 x->y 简单路径上报价最小值的最小值；C x y 改报价
string solve8(istream& in) {
    int n, m, q;
    in >> n >> m >> q;
    vector<int> price(n + 1);
    for (int i = 1; i <= n; i++) in >> price[i];
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    string out;
    while (q--) {
        char op; int x, y;
        in >> op >> x >> y;
        if (op == 'C') {
            price[x] = y;
        } else {
            // 枚举 x->y 简单路径
            vector<char> used(n + 1, 0);
            vector<int> cur;
            int best = 1e9;
            function<void(int)> dfs = [&](int u) {
                if (u == y) {
                    int mn = 1e9;
                    for (int v : cur) mn = min(mn, price[v]);
                    best = min(best, mn);
                    return;
                }
                for (auto& p : g.adj[u]) {
                    int v = p.first;
                    if (!used[v]) {
                        used[v] = 1; cur.push_back(v);
                        dfs(v);
                        cur.pop_back(); used[v] = 0;
                    }
                }
            };
            used[x] = 1; cur.push_back(x);
            dfs(x);
            out += to_string(best) + "\n";
        }
    }
    return out;
}

// 题9：战略游戏（多测 t）。每查询 s 关键点，数可选点 v（v∉S，删 v 后 S 中有两点不连通）
string solve9(istream& in) {
    int t;
    in >> t;
    string out;
    while (t--) {
        int n, m;
        in >> n >> m;
        G g(n);
        for (int i = 1; i <= m; i++) {
            int u, v; in >> u >> v;
            g.adj[u].push_back({v, i});
            g.adj[v].push_back({u, i});
        }
        int q; in >> q;
        while (q--) {
            int s; in >> s;
            vector<int> keys(s);
            vector<char> isKey(n + 1, 0);
            for (int i = 0; i < s; i++) { in >> keys[i]; isKey[keys[i]] = 1; }
            long long ans = 0;
            for (int v = 1; v <= n; v++) {
                if (isKey[v]) continue;
                delV.assign(n + 1, 0);
                delE.assign(m + 1, 0);
                delV[v] = 1;
                vector<int> verts = keys;
                if (!connected(g, verts)) ans++;
            }
            out += to_string(ans) + "\n";
        }
    }
    return out;
}

// 题10：国土规划。初始全 0；翻转 x；打印无所谓点数量（状态0且删后 1 点仍连通）
string solve10(istream& in) {
    int n, m, q;
    in >> n >> m >> q;
    G g(n);
    for (int i = 1; i <= m; i++) {
        int u, v; in >> u >> v;
        g.adj[u].push_back({v, i});
        g.adj[v].push_back({u, i});
    }
    vector<char> st(n + 1, 0);
    string out;
    while (q--) {
        int x; in >> x;
        st[x] = !st[x];
        long long ans = 0;
        vector<int> ones;
        for (int i = 1; i <= n; i++) if (st[i]) ones.push_back(i);
        for (int v = 1; v <= n; v++) {
            if (st[v]) continue;
            delV.assign(n + 1, 0);
            delE.assign(m + 1, 0);
            delV[v] = 1;
            if (connected(g, ones)) ans++;
        }
        out += to_string(ans) + "\n";
    }
    return out;
}

int main(int argc, char** argv) {
    int w = argc > 1 ? atoi(argv[1]) : 1;
    switch (w) {
        case 1: cout << solve1(cin); break;
        case 2: cout << solve2(cin); break;
        case 3: cout << solve3(cin); break;
        case 4: cout << solve4(cin); break;
        case 5: cout << solve5(cin); break;
        case 6: cout << solve6(cin); break;
        case 7: cout << solve7(cin); break;
        case 8: cout << solve8(cin); break;
        case 9: cout << solve9(cin); break;
        case 10: cout << solve10(cin); break;
    }
    return 0;
}
