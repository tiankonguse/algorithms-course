#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int which;
int n, m;

// ============ 题1 割点模版：删点后连通块数增加 ============
void solve1() {
    cin >> n >> m;
    vector<pair<int,int>> es(m);
    for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
    auto comps = [&](const vector<int>& del) {
        vector<vector<int>> g(n + 1);
        for (auto& e : es) {
            if (del[e.first] || del[e.second]) continue;
            g[e.first].push_back(e.second);
            g[e.second].push_back(e.first);
        }
        int c = 0;
        vector<int> vis(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            if (del[i] || vis[i]) continue;
            c++;
            queue<int> q; q.push(i); vis[i] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : g[u]) if (!vis[v]) { vis[v] = 1; q.push(v); }
            }
        }
        return c;
    };
    int base = comps(vector<int>(n + 1, 0));
    vector<int> cut;
    for (int v = 1; v <= n; v++) {
        vector<int> del(n + 1, 0);
        del[v] = 1;
        if (comps(del) > base) cut.push_back(v);
    }
    cout << cut.size() << "\n";
    for (int v : cut) cout << v << " ";
    cout << "\n";
}

// ============ 题2 电力：删一点最多连通块 ============
void solve2() {
    while (cin >> n >> m) {
        if (n == 0 && m == 0) break;
        vector<pair<int,int>> es(m);
        for (int i = 0; i < m; i++) { cin >> es[i].first >> es[i].second; es[i].first++; es[i].second++; }
        auto comps = [&](const vector<int>& del) {
            vector<vector<int>> g(n + 1);
            for (auto& e : es) {
                if (del[e.first] || del[e.second]) continue;
                g[e.first].push_back(e.second);
                g[e.second].push_back(e.first);
            }
            int c = 0;
            vector<int> vis(n + 1, 0);
            for (int i = 1; i <= n; i++) {
                if (del[i] || vis[i]) continue;
                c++;
                queue<int> q; q.push(i); vis[i] = 1;
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    for (int v : g[u]) if (!vis[v]) { vis[v] = 1; q.push(v); }
                }
            }
            return c;
        };
        int C = comps(vector<int>(n + 1, 0));
        int best = 0;
        for (int v = 1; v <= n; v++) {
            vector<int> del(n + 1, 0);
            del[v] = 1;
            int kv = comps(del); // 删 v 后总块数
            // 新增 = kv - (C - 1)；课程 ans 记录的是删后所在区块数 k，总块 = C - 1 + k
            best = max(best, kv);
        }
        cout << best << "\n";
        cin >> n >> m;
    }
}

// ============ 题3 嗅探器：删 v 使 a、b 不连通，最小编号 ============
void solve3() {
    cin >> n;
    vector<pair<int,int>> es;
    int u, v;
    while (cin >> u >> v, u != 0 || v != 0) es.push_back({u, v});
    int a, b; cin >> a >> b;
    auto conn = [&](const vector<int>& del, int s, int t) {
        vector<vector<int>> g(n + 1);
        for (auto& e : es) {
            if (del[e.first] || del[e.second]) continue;
            g[e.first].push_back(e.second);
            g[e.second].push_back(e.first);
        }
        vector<int> vis(n + 1, 0);
        queue<int> q; q.push(s); vis[s] = 1;
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int y : g[x]) if (!vis[y]) { vis[y] = 1; q.push(y); }
        }
        return vis[t];
    };
    int ans = 0;
    for (int v = 1; v <= n && ans == 0; v++) {
        if (v == a || v == b) continue;
        vector<int> del(n + 1, 0);
        del[v] = 1;
        if (!conn(del, a, b)) ans = v;
    }
    if (ans == 0) cout << "No solution\n";
    else cout << ans << "\n";
}

// ============ 点双通用：极大无割点连通点集 + 孤立点 ============
vector<vector<int>> vbccBrute(const vector<pair<int,int>>& es, int n, bool with_isolated) {
    vector<vector<int>> g(n + 1);
    vector<vector<int>> adj(n + 1);
    for (auto& e : es) {
        if (e.first == e.second) continue; // 忽略自环
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    int full = 1 << n;
    vector<char> good(full, 0);
    for (int S = 1; S < full; S++) {
        vector<int> nodes;
        for (int i = 0; i < n; i++) if (S >> i & 1) nodes.push_back(i + 1);
        int sz = nodes.size();
        if (sz == 1) {
            // 孤立点（无边）才成单块
            if (adj[nodes[0]].empty()) good[S] = 1;
            continue;
        }
        // 导出子图连通
        auto inS = [&](int x) { return S >> (x - 1) & 1; };
        int start = nodes[0];
        queue<int> q; q.push(start);
        vector<char> vis(n + 1, 0); vis[start] = 1;
        int cnt = 1;
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int y : adj[x]) if (inS(y) && !vis[y]) { vis[y] = 1; cnt++; q.push(y); }
        }
        if (cnt != sz) continue;
        // 无割点：删任意点仍连通
        bool ok = true;
        for (int v : nodes) {
            vector<char> vis2(n + 1, 0);
            int st = (v == start) ? nodes[1] : start;
            queue<int> q2; q2.push(st); vis2[st] = 1;
            int c2 = 1;
            while (!q2.empty()) {
                int x = q2.front(); q2.pop();
                for (int y : adj[x]) if (inS(y) && y != v && !vis2[y]) { vis2[y] = 1; c2++; q2.push(y); }
            }
            if (c2 != sz - 1) { ok = false; break; }
        }
        if (ok) good[S] = 1;
    }
    // 极大化：按大小降序取，交集 ≥2 跳过
    vector<int> order;
    for (int S = 1; S < full; S++) if (good[S]) order.push_back(S);
    sort(order.begin(), order.end(), [](int a, int b) { return __builtin_popcount(a) > __builtin_popcount(b); });
    vector<vector<int>> res;
    for (int S : order) {
        vector<int> nodes;
        for (int i = 0; i < n; i++) if (S >> i & 1) nodes.push_back(i + 1);
        bool merged = false;
        for (auto& blk : res) {
            int inter = 0;
            for (int x : blk) if (S >> (x - 1) & 1) inter++;
            if (inter >= 2) { merged = true; break; }
        }
        if (!merged) res.push_back(nodes);
    }
    if (!with_isolated) {
        vector<vector<int>> tmp;
        for (auto& blk : res) if (blk.size() > 1 || !adj[blk[0]].empty()) tmp.push_back(blk);
        res = tmp;
    }
    return res;
}

// ============ 题4 点双模版1 ============
void solve4() {
    cin >> n >> m;
    vector<pair<int,int>> es(m);
    for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
    auto blocks = vbccBrute(es, n, true);
    cout << blocks.size() << "\n";
    for (auto& blk : blocks) {
        sort(blk.begin(), blk.end());
        cout << blk.size() << "\n";
        for (int x : blk) cout << x << " ";
        cout << "\n";
    }
}

// ============ 题5 点双模版2（字典序） ============
void solve5() {
    cin >> n >> m;
    vector<pair<int,int>> es(m);
    for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
    auto blocks = vbccBrute(es, n, false);
    for (auto& blk : blocks) sort(blk.begin(), blk.end());
    sort(blocks.begin(), blocks.end());
    cout << blocks.size() << "\n";
    for (auto& blk : blocks) {
        for (int x : blk) cout << x << " ";
        cout << "\n";
    }
}

// ============ 题6 夺回据点：枚举夺取顺序 ============
void solve6() {
    cin >> n >> m;
    vector<pair<int,int>> es(m);
    for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second; // 0-indexed
    vector<ll> cost(n);
    for (int i = 0; i < n; i++) cin >> cost[i];
    vector<vector<int>> adj(n);
    for (auto& e : es) { adj[e.first].push_back(e.second); adj[e.second].push_back(e.first); }
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    ll best = LLONG_MAX;
    auto unTakenConnected = [&](const vector<char>& taken, int remain) {
        if (remain == 0) return true;
        int st = -1;
        for (int i = 0; i < n; i++) if (!taken[i]) { st = i; break; }
        queue<int> q; q.push(st);
        vector<char> vis(n, 0); vis[st] = 1;
        int cnt = 1;
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int y : adj[x]) if (!taken[y] && !vis[y]) { vis[y] = 1; cnt++; q.push(y); }
        }
        return cnt == remain;
    };
    do {
        vector<char> taken(n, 0);
        int remain = n;
        ll tot = 0;
        bool ok = true;
        for (int x : perm) {
            // 夺取 x：与已夺集合相邻则免费
            bool free = false;
            for (int y : adj[x]) if (taken[y]) { free = true; break; }
            if (!free) tot += cost[x];
            taken[x] = 1;
            remain--;
            if (!unTakenConnected(taken, remain)) { ok = false; break; }
        }
        if (ok) best = min(best, tot);
    } while (next_permutation(perm.begin(), perm.end()));
    cout << best << "\n";
}

// ============ 题7 矿场搭建：救援点集合 ============
void solve7() {
    int t = 0;
    while (cin >> m) {
        if (m == 0) break;
        vector<pair<int,int>> es(m);
        n = 0;
        for (int i = 0; i < m; i++) { cin >> es[i].first >> es[i].second; n = max(n, max(es[i].first, es[i].second)); }
        vector<vector<int>> adj(n + 1);
        for (auto& e : es) { adj[e.first].push_back(e.second); adj[e.second].push_back(e.first); }
        ll ans1 = n + 1, ans2 = 0;
        for (int S = 1; S < (1 << n); S++) {
            int cnt = __builtin_popcount(S);
            vector<char> rescue(n + 1, 0);
            for (int i = 0; i < n; i++) if (S >> i & 1) rescue[i + 1] = 1;
            bool ok = true;
            for (int v = 1; v <= n && ok; v++) {
                // 地震 v：v 的救援点失效，检查剩余点每块有救援点
                vector<char> vis(n + 1, 0);
                for (int x = 1; x <= n; x++) {
                    if (x == v || vis[x]) continue;
                    queue<int> q; q.push(x); vis[x] = 1;
                    bool hasR = false;
                    while (!q.empty()) {
                        int u = q.front(); q.pop();
                        if (rescue[u]) hasR = true;
                        for (int y : adj[u]) if (y != v && !vis[y]) { vis[y] = 1; q.push(y); }
                    }
                    if (!hasR) { ok = false; break; }
                }
            }
            if (ok) {
                if (cnt < ans1) { ans1 = cnt; ans2 = 1; }
                else if (cnt == ans1) ans2++;
            }
        }
        cout << "Case " << (++t) << ": " << ans1 << " " << ans2 << "\n";
        cin >> m;
    }
}

// ============ 题8 圆桌骑士：存在经过 u 的奇环 ============
void solve8() {
    while (cin >> n >> m) {
        if (n == 0 && m == 0) break;
        vector<vector<char>> hate(n + 1, vector<char>(n + 1, 0));
        for (int i = 0; i < m; i++) {
            int u, v; cin >> u >> v;
            hate[u][v] = hate[v][u] = 1;
        }
        vector<vector<int>> adj(n + 1);
        for (int u = 1; u <= n; u++)
            for (int v = u + 1; v <= n; v++)
                if (!hate[u][v]) { adj[u].push_back(v); adj[v].push_back(u); }
        auto inCycle = [&](int u) {
            int full = 1 << n;
            for (int S = 1; S < full; S++) {
                if (!(S >> (u - 1) & 1)) continue;
                int sz = __builtin_popcount(S);
                if (sz < 3 || sz % 2 == 0) continue;
                // 导出子图每个点度 2 且连通 → 环
                bool ok = true;
                int deg0 = 0;
                for (int i = 1; i <= n; i++) {
                    if (!(S >> (i - 1) & 1)) continue;
                    int d = 0;
                    for (int j : adj[i]) if (S >> (j - 1) & 1) d++;
                    if (d != 2) { ok = false; break; }
                    deg0 = i;
                }
                if (!ok) continue;
                // 连通性（度 2 且边数 = 点数，无重边 → 每个分量是环；需单环）
                queue<int> q; q.push(deg0);
                vector<char> vis(n + 1, 0); vis[deg0] = 1;
                int c = 1;
                while (!q.empty()) {
                    int x = q.front(); q.pop();
                    for (int j : adj[x]) if ((S >> (j - 1) & 1) && !vis[j]) { vis[j] = 1; c++; q.push(j); }
                }
                if (c == sz) return true;
            }
            return false;
        };
        int ans = 0;
        for (int i = 1; i <= n; i++) if (!inCycle(i)) ans++;
        cout << ans << "\n";
        cin >> n >> m;
    }
}

int main(int argc, char** argv) {
    which = atoi(argv[1]);
    switch (which) {
        case 1: solve1(); break;
        case 2: solve2(); break;
        case 3: solve3(); break;
        case 4: solve4(); break;
        case 5: solve5(); break;
        case 6: solve6(); break;
        case 7: solve7(); break;
        case 8: solve8(); break;
    }
    return 0;
}
