#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<pair<int,int>> es;   // 1-based
vector<int> eid_keep;

// ---------- 公共：桥判定（枚举删边 BFS） ----------
vector<vector<pair<int,int>>> g;
bool is_bridge(int eid){
    int s = es[eid].first, t = es[eid].second;
    if (s == t) return false;
    vector<int> vis(n + 1, 0); queue<int> q;
    q.push(s); vis[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto& pr : g[u]) {
            int v = pr.first, id = pr.second;
            if (id / 2 == eid) continue;
            if (!vis[v]) { vis[v] = 1; q.push(v); }
        }
    }
    return !vis[t];
}
vector<int> all_bridges(){
    vector<int> b;
    for (int i = 0; i < m; i++) if (is_bridge(i)) b.push_back(i);
    return b;
}

// ---------- 公共：删桥分块（缩点） ----------
vector<int> belong;   // 1..n -> 0..ebccCnt-1
int ebccCnt;
vector<vector<int>> tree;   // 缩点树邻接（去重）
vector<pair<int,int>> treeEdges;  // (u,v) 树边（缩点编号）
void condense(){
    vector<int> br = all_bridges();
    vector<bool> isbr(m, false);
    for (int x : br) isbr[x] = true;
    belong.assign(n + 1, -1);
    ebccCnt = 0;
    for (int i = 1; i <= n; i++) {
        if (belong[i] != -1) continue;
        vector<int> vis(n + 1, 0); queue<int> q;
        q.push(i); vis[i] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            belong[u] = ebccCnt;
            for (auto& pr : g[u]) {
                int v = pr.first, id = pr.second;
                if (isbr[id / 2]) continue;
                if (!vis[v]) { vis[v] = 1; q.push(v); }
            }
        }
        ebccCnt++;
    }
    tree.assign(ebccCnt, {});
    treeEdges.clear();
    for (int i = 0; i < m; i++) {
        if (!isbr[i]) continue;
        int a = belong[es[i].first], b = belong[es[i].second];
        if (a != b) {
            tree[a].push_back(b); tree[b].push_back(a);
            treeEdges.push_back({min(a,b), max(a,b)});
        }
    }
    // 去重树边
    sort(treeEdges.begin(), treeEdges.end());
    treeEdges.erase(unique(treeEdges.begin(), treeEdges.end()), treeEdges.end());
}
int tree_dist(int a, int b){  // 缩点树上 BFS 距离
    vector<int> d(ebccCnt, -1); queue<int> q;
    q.push(a); d[a] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : tree[u]) if (d[v] == -1) { d[v] = d[u] + 1; q.push(v); }
    }
    return d[b];
}
vector<int> tree_path(int a, int b){  // 缩点树路径节点（含端点）
    vector<int> par(ebccCnt, -1); queue<int> q;
    q.push(a); par[a] = a;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : tree[u]) if (par[v] == -1) { par[v] = u; q.push(v); }
    }
    vector<int> p;
    for (int x = b; x != a; x = par[x]) p.push_back(x);
    p.push_back(a);
    return p;
}
string toBinary(int x){
    if (x == 0) return "0";
    string s;
    while (x > 0) { s.push_back((x & 1) ? '1' : '0'); x >>= 1; }
    reverse(s.begin(), s.end());
    return s;
}

int main(int argc, char** argv){
    int which = argc > 1 ? atoi(argv[1]) : 1;
    cin >> n >> m;
    es.resize(m);
    if (which == 1) {
        vector<int> w(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second >> w[i];
        int s, t; cin >> s >> t;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        int S = belong[s], T = belong[t];
        vector<int> path = tree_path(S, T);
        vector<int> on(ebccCnt, 0);
        for (int x : path) on[x] = 1;
        bool ok = false;
        // 桥上的商品
        for (auto& e : treeEdges) {
            // 树边是否在 S-T 路径上：路径节点集合中两节点相邻
            // 简化：S-T 路径的每条边都是树边，直接查路径
        }
        // 更简单：对每条商品边判断
        for (int i = 0; i < m; i++) {
            if (w[i] != 1) continue;
            int u = es[i].first, v = es[i].second;
            if (belong[u] == belong[v]) {
                if (on[belong[u]]) { ok = true; break; }
            } else {
                // 桥：检查 belong 两端的树边是否在路径上
                int a = belong[u], b = belong[v];
                if (min(a,b) == max(a,b)) continue;
                // 路径上相邻两节点
                for (int k = 0; k + 1 < (int)path.size(); k++) {
                    if ((path[k] == a && path[k+1] == b) || (path[k] == b && path[k+1] == a)) { ok = true; break; }
                }
                if (ok) break;
            }
        }
        cout << (ok ? "YES" : "NO") << "\n";
    } else if (which == 2) {
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        int best = 0;
        for (int i = 0; i < ebccCnt; i++)
            for (int j = i + 1; j < ebccCnt; j++)
                best = max(best, tree_dist(i, j));
        cout << best << "\n";
    } else if (which == 3) {
        vector<int> c(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second >> c[i];
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        int maxv = 0; for (int x : c) maxv = max(maxv, x);
        auto check = [&](int limit) {
            // 费用<=limit 的桥视为权1；带权直径 < 总数
            int cnt = 0;
            for (auto& e : treeEdges) {
                // 找这条树边对应的原桥费用
                int w = -1;
                for (int i = 0; i < m; i++) {
                    int a = belong[es[i].first], b = belong[es[i].second];
                    if (a == e.first && b == e.second || a == e.second && b == e.first) { if (w == -1 || c[i] < w) w = c[i]; }
                }
                if (w <= limit) cnt++;
            }
            // 带权直径：全点对带权距离（树边权 = 该桥费用<=limit ? 1 : 0）
            int dia = 0;
            vector<vector<pair<int,int>>> wtr(ebccCnt);
            for (auto& e : treeEdges) {
                int w = -1;
                for (int i = 0; i < m; i++) {
                    int a = belong[es[i].first], b = belong[es[i].second];
                    if ((a == e.first && b == e.second) || (a == e.second && b == e.first)) { if (w == -1 || c[i] < w) w = c[i]; }
                }
                int ww = (w <= limit) ? 1 : 0;
                wtr[e.first].push_back({e.second, ww});
                wtr[e.second].push_back({e.first, ww});
            }
            for (int i = 0; i < ebccCnt; i++)
                for (int j = i + 1; j < ebccCnt; j++) {
                    vector<int> d(ebccCnt, -1); queue<int> q;
                    q.push(i); d[i] = 0;
                    while (!q.empty()) {
                        int u = q.front(); q.pop();
                        for (auto& pr : wtr[u]) if (d[pr.first] == -1) { d[pr.first] = d[u] + pr.second; q.push(pr.first); }
                    }
                    dia = max(dia, d[j]);
                }
            return dia < cnt;
        };
        int l = 1, r = maxv, ans = -1;
        while (l <= r) { int mid = (l + r) / 2; if (check(mid)) { ans = mid; r = mid - 1; } else l = mid + 1; }
        cout << ans << "\n";
    } else if (which == 4) {
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        // 题面：两点间重边只连一次，按端点去重
        for (int i = 0; i < m; i++) {
            int u = es[i].first, v = es[i].second;
            es[i] = {min(u, v), max(u, v)};
        }
        sort(es.begin(), es.end());
        es.erase(unique(es.begin(), es.end()), es.end());
        m = es.size();
        int q; cin >> q;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        for (int i = 0; i < q; i++) {
            int x, y; cin >> x >> y;
            int a = belong[x], b = belong[y];
            int d = (a == b) ? 1 : tree_dist(a, b) + 1;
            cout << toBinary(d) << "\n";
        }
    } else if (which == 5) {
        vector<int> arr(n + 1);
        for (int i = 1; i <= n; i++) cin >> arr[i];
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        int q; cin >> q;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        vector<int> sum(ebccCnt, 0);
        for (int i = 1; i <= n; i++) sum[belong[i]] += arr[i];
        vector<int> cov(ebccCnt, 0);
        for (int i = 0; i < q; i++) {
            int x, y; cin >> x >> y;
            int a = belong[x], b = belong[y];
            vector<int> p = tree_path(a, b);
            for (int z : p) cov[z] = 1;
        }
        int ans = 0;
        for (int i = 0; i < ebccCnt; i++) if (cov[i]) ans += sum[i];
        cout << ans << "\n";
    } else if (which == 6) {
        int q; cin >> q;
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        // 缩点森林：每连通块 BFS 定根/父/深
        vector<int> par(ebccCnt, -1), dep(ebccCnt, 0), rootOf(ebccCnt, -1);
        int bc = 0;
        for (int i = 0; i < ebccCnt; i++) if (rootOf[i] == -1) {
            queue<int> q2; q2.push(i); par[i] = i; rootOf[i] = bc;
            while (!q2.empty()) {
                int u = q2.front(); q2.pop();
                for (int v : tree[u]) if (rootOf[v] == -1) {
                    rootOf[v] = bc; par[v] = u; dep[v] = dep[u] + 1; q2.push(v);
                }
            }
            bc++;
        }
        vector<int> upCnt(ebccCnt, 0), downCnt(ebccCnt, 0);
        bool ok = true;
        for (int i = 0; i < q; i++) {
            int x, y; cin >> x >> y;
            int a = belong[x], b = belong[y];
            if (rootOf[a] != rootOf[b]) { ok = false; continue; }
            // 暴力求 LCA
            int u = a, v = b;
            while (dep[u] > dep[v]) u = par[u];
            while (dep[v] > dep[u]) v = par[v];
            while (u != v) { u = par[u]; v = par[v]; }
            int lca = u;
            // x 侧链：x(=a) 爬到 lca，路径方向 子->父 = up
            for (int cur = a; cur != lca; cur = par[cur]) upCnt[cur]++;
            // y 侧链：lca 到 b，路径方向 父->子 = down（边是 b 爬回 lca 的链，方向要求朝 b）
            for (int cur = b; cur != lca; cur = par[cur]) downCnt[cur]++;
        }
        for (int i = 0; i < ebccCnt; i++)
            if (par[i] != i && upCnt[i] > 0 && downCnt[i] > 0) ok = false;
        cout << (ok ? "Yes" : "No") << "\n";
    } else if (which == 7) {
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) { g[es[i].first].push_back({es[i].second, i * 2}); g[es[i].second].push_back({es[i].first, i * 2 + 1}); }
        condense();
        const long long MOD = 1000000007;
        long long pow2[64];
        pow2[0] = 1;
        for (int i = 1; i < 64; i++) pow2[i] = pow2[i - 1] * 2 % MOD;
        // 枚举军营城市集合 S（原图点）
        long long ans = 0;
        for (int S = 1; S < (1 << n); S++) {
            vector<int> nodes;
            for (int i = 1; i <= n; i++) if (S >> (i - 1) & 1) nodes.push_back(i);
            vector<int> comps;
            for (int x : nodes) comps.push_back(belong[x]);
            sort(comps.begin(), comps.end());
            comps.erase(unique(comps.begin(), comps.end()), comps.end());
            // 必经桥：删掉后 comps 内两点不连通
            int must = 0;
            for (auto& e : treeEdges) {
                // 删掉 e 后 comps 连通性
                vector<int> par(ebccCnt, -1); queue<int> q2;
                q2.push(comps[0]); par[comps[0]] = comps[0];
                while (!q2.empty()) {
                    int u = q2.front(); q2.pop();
                    for (int v : tree[u]) {
                        if (min(u,v) == e.first && max(u,v) == e.second) continue;
                        if (min(u,v) == e.second && max(u,v) == e.first) continue;
                        if (par[v] == -1) { par[v] = u; q2.push(v); }
                    }
                }
                for (int c : comps) if (par[c] == -1) { must++; break; }
            }
            // 看守集合必须包含 must 条必经桥；其余 m-must 条原边自由
            ans = (ans + pow2[m - must]) % MOD;
        }
        cout << ans << "\n";
    }
    return 0;
}
