// 191 课统一暴力（独立于课程实现）：atoi(argv[1]) 题号
// 割边判定 = 枚举删边 BFS（不依赖 tarjan）
// 题1: 割边数量 + 序号升序；题2: 端点对（输入序，0-based）
// 题3: 关键边端点对（v 侧在前）；题5: 边双（删割边 BFS 分量，无序）
// 题6: 每次加边后割边数（Case 格式）；题7: (leaf+1)/2
#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<pair<int, int>> es;  // 输入序 1-based
vector<vector<pair<int, int>>> g;  // g[u] = {(v, eid)}，无向边 eid*2 / eid*2+1 两条有向边

void build() {
    g.assign(n + 1, {});
    for (int i = 0; i < m; i++) {
        int u = es[i].first, v = es[i].second;
        g[u].push_back({v, i * 2});
        g[v].push_back({u, i * 2 + 1});
    }
}

// 删掉无向边 eid 后 u、v 是否还连通
bool connected_without(int eid) {
    int s = es[eid].first, t = es[eid].second;
    if (s == t) return true;  // 自环：删掉不影响自身连通
    vector<int> vis(n + 1, 0);
    queue<int> q;
    q.push(s); vis[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto& pr : g[u]) {
            int v = pr.first, id = pr.second;
            if (id / 2 == eid) continue;  // 该无向边禁用
            if (!vis[v]) { vis[v] = 1; q.push(v); }
        }
    }
    return vis[t];
}

bool is_bridge(int eid) { return !connected_without(eid); }

// 从 s 出发不经过边 eid 的连通块点集
vector<int> comp_without(int eid, int s) {
    vector<int> vis(n + 1, 0), res;
    queue<int> q;
    q.push(s); vis[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for (auto& pr : g[u]) {
            int v = pr.first, id = pr.second;
            if (id / 2 == eid) continue;
            if (!vis[v]) { vis[v] = 1; q.push(v); }
        }
    }
    return res;
}

// 求全部割边（输入序 id 列表）
vector<int> all_bridges() {
    vector<int> res;
    for (int i = 0; i < m; i++) if (is_bridge(i)) res.push_back(i);
    return res;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int which = atoi(argv[1]);

    if (which == 6) {
        // 多组 Case
        while (cin >> n >> m) {
            if (n == 0 && m == 0) break;
            es.resize(m);
            for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
            build();
            int q; cin >> q;
            cout << "Case " << 1 << ":\n";
            for (int i = 0; i < q; i++) {
                int x, y; cin >> x >> y;
                es.push_back({x, y}); m++;
                g.assign(n + 1, {});
                for (int j = 0; j < m; j++) {
                    int u = es[j].first, v = es[j].second;
                    g[u].push_back({v, j * 2});
                    g[v].push_back({u, j * 2 + 1});
                }
                cout << (int)all_bridges().size() << "\n";
            }
            cout << "\n";
        }
        return 0;
    }

    cin >> n >> m;
    if (which == 1) {
        es.resize(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        build();
        vector<int> b = all_bridges();
        cout << b.size() << "\n";
        for (size_t i = 0; i < b.size(); i++) cout << (b[i] + 1) << (i + 1 == b.size() ? "\n" : " ");
    }
    else if (which == 2) {
        es.resize(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        build();
        for (int i = 0; i < m; i++) if (is_bridge(i)) cout << es[i].first << " " << es[i].second << "\n";
    }
    else if (which == 3) {
        // 输入：n m k l，k 行 A，l 行 B，m 行边
        int k, l; cin >> k >> l;
        vector<int> acnt(n + 1, 0), bcnt(n + 1, 0);
        for (int i = 0; i < k; i++) { int x; cin >> x; acnt[x] = 1; }
        for (int i = 0; i < l; i++) { int x; cin >> x; bcnt[x] = 1; }
        es.resize(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        build();
        // dfn：从 1 开始 DFS（连通图）
        vector<int> dfn(n + 1, 0);
        int cntd = 0;
        vector<int> order;  // 访问序
        vector<int> stk; stk.push_back(1);
        vector<int> it(n + 1, 0);
        dfn[1] = ++cntd; order.push_back(1);
        // 迭代 DFS 记 dfn（只需 dfn，不用 low）
        while (!stk.empty()) {
            int u = stk.back();
            bool done = true;
            for (int &p = it[u]; p < (int)g[u].size(); p++) {
                int v = g[u][p].first, eid = g[u][p].second / 2;
                if (!dfn[v]) { dfn[v] = ++cntd; order.push_back(v); stk.push_back(v); done = false; break; }
            }
            if (done) stk.pop_back();
        }
        vector<pair<int, int>> out;  // (子侧点, 父侧点)
        for (int i = 0; i < m; i++) {
            if (!is_bridge(i)) continue;
            int u = es[i].first, v = es[i].second;
            // 子树侧 = dfn 大者；从该点出发不经过边 i 的块
            int root = (dfn[u] > dfn[v]) ? u : v;
            int other = (root == u) ? v : u;
            auto c = comp_without(i, root);
            int a = 0, b = 0;
            for (int x : c) { a += acnt[x]; b += bcnt[x]; }
            if (a == 0 || a == k || b == 0 || b == l) {
                out.push_back({root, other});
            }
        }
        cout << out.size() << "\n";
        for (auto& pr : out) cout << pr.first << " " << pr.second << "\n";
    }
    else if (which == 4) {
        cout << "UNUSED\n";
    }
    else if (which == 5) {
        es.resize(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        build();
        vector<int> b = all_bridges();
        vector<vector<pair<int, int>>> eg = g;  // 拷贝后删除割边
        // 重新建图但跳过割边
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) {
            if (find(b.begin(), b.end(), i) != b.end()) continue;
            int u = es[i].first, v = es[i].second;
            g[u].push_back({v, i});
            g[v].push_back({u, i});
        }
        vector<int> vis(n + 1, 0);
        vector<vector<int>> comps;
        for (int s = 1; s <= n; s++) {
            if (vis[s]) continue;
            vector<int> c;
            queue<int> q; q.push(s); vis[s] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                c.push_back(u);
                for (auto& pr : g[u]) if (!vis[pr.first]) { vis[pr.first] = 1; q.push(pr.first); }
            }
            comps.push_back(c);
        }
        cout << comps.size() << "\n";
        for (auto& c : comps) {
            cout << c.size() << "\n";
            for (int x : c) cout << x << " ";
            cout << "\n";
        }
    }
    else if (which == 7) {
        es.resize(m);
        for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
        build();
        vector<int> b = all_bridges();
        g.assign(n + 1, {});
        for (int i = 0; i < m; i++) {
            if (find(b.begin(), b.end(), i) != b.end()) continue;
            int u = es[i].first, v = es[i].second;
            g[u].push_back({v, i});
            g[v].push_back({u, i});
        }
        vector<int> belong(n + 1, 0);
        int comps = 0;
        for (int s = 1; s <= n; s++) {
            if (belong[s]) continue;
            comps++;
            queue<int> q; q.push(s); belong[s] = comps;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (auto& pr : g[u]) if (!belong[pr.first]) { belong[pr.first] = comps; q.push(pr.first); }
            }
        }
        vector<int> deg(comps + 1, 0);
        for (int i = 0; i < m; i++) {
            int a = belong[es[i].first], b2 = belong[es[i].second];
            if (a != b2) { deg[a]++; deg[b2]++; }
        }
        int leaf = 0;
        for (int i = 1; i <= comps; i++) if (deg[i] == 1) leaf++;
        cout << (leaf + 1) / 2 << "\n";
    }
    return 0;
}
