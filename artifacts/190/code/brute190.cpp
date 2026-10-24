// 190 课统一暴力：缩点（Floyd 闭包聚类）+ 各题独立计算
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n;
vector<vector<bool>> R;
vector<int> belong;
vector<vector<int>> sccs;

// 闭包 + 聚类（与课程 Tarjan 等价，暴力用）
void buildSCC() {
    for (int k2 = 1; k2 <= n; k2++)
        for (int i = 1; i <= n; i++) if (R[i][k2])
            for (int j = 1; j <= n; j++) if (R[k2][j]) R[i][j] = true;
    vector<bool> used(n + 1, false);
    belong.assign(n + 1, -1);
    sccs.clear();
    for (int i = 1; i <= n; i++) {
        if (used[i]) continue;
        vector<int> g;
        for (int j = 1; j <= n; j++) if (!used[j] && R[i][j] && R[j][i]) g.push_back(j);
        for (int x : g) { used[x] = true; belong[x] = sccs.size(); }
        sort(g.begin(), g.end());
        sccs.push_back(g);
    }
}

int main(int argc, char** argv) {
    int which = atoi(argv[1]);
    if (which == 1) {
        int m; cin >> n >> m;
        vector<int> arr(n + 1);
        for (int i = 1; i <= n; i++) cin >> arr[i];
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 0; i < m; i++) { int a, b; cin >> a >> b; R[a][b] = true; es.push_back({a,b}); }
        buildSCC();
        int k = sccs.size();
        vector<int> sum(k, 0);
        for (int a = 0; a < k; a++) for (int x : sccs[a]) sum[a] += arr[x];
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) { g[a].push_back(b); }
        }
        // 去重（缩点图 DAG 上重边不影响取 max，但 Kahn 需要入度唯一计数）
        for (int a = 0; a < k; a++) { sort(g[a].begin(), g[a].end()); g[a].erase(unique(g[a].begin(), g[a].end()), g[a].end()); }
        for (int a = 0; a < k; a++) for (int b : g[a]) indeg[b]++;
        queue<int> q;
        vector<int> dp(k, 0);
        for (int i = 0; i < k; i++) if (indeg[i] == 0) { q.push(i); dp[i] = sum[i]; }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                dp[v] = max(dp[v], dp[u] + sum[v]);
                if (--indeg[v] == 0) q.push(v);
            }
        }
        int ans = 0;
        for (int i = 0; i < k; i++) ans = max(ans, dp[i]);
        cout << ans << "\n";
    }
    else if (which == 2) {
        int m; cin >> n >> m;
        vector<int> init(m + 1), rec(m + 1);
        vector<int> a(m + 1), b(m + 1);
        double rd;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 1; i <= m; i++) { cin >> a[i] >> b[i] >> init[i] >> rd; rec[i] = (int)(rd * 10); R[a[i]][b[i]] = true; }
        int s; cin >> s;
        buildSCC();
        int k = sccs.size();
        vector<int> sum(k, 0);
        vector<vector<pair<int,int>>> g(k);
        for (int i = 1; i <= m; i++) {
            int s1 = belong[a[i]], s2 = belong[b[i]];
            if (s1 == s2) {
                int val = init[i], r = rec[i];
                while (val > 0) { sum[s1] += val; val = val * r / 10; }
            } else {
                g[s1].push_back({s2, init[i]});
            }
        }
        // Kahn 最长路（初值只从 s 所在分量起）
        vector<int> indeg(k, 0);
        for (int u = 0; u < k; u++) for (auto& e : g[u]) indeg[e.first]++;
        queue<int> q;
        vector<int> dp(k, INT_MIN / 2);
        int st = belong[s];
        dp[st] = sum[st];
        for (int i = 0; i < k; i++) if (indeg[i] == 0) q.push(i);
        // 注意：入度 0 可能不是 st；直接按拓扑序处理，dp 初始只在 st
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : g[u]) {
                int v = e.first, w = e.second;
                if (dp[u] != INT_MIN / 2) dp[v] = max(dp[v], dp[u] + w + sum[v]);
                if (--indeg[v] == 0) q.push(v);
            }
        }
        int ans = 0;
        for (int i = 0; i < k; i++) ans = max(ans, dp[i]);
        cout << ans << "\n";
    }
    else if (which == 3) {
        int m; cin >> n >> m;
        vector<int> money(n + 1);
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 0; i < m; i++) { int a, b; cin >> a >> b; R[a][b] = true; es.push_back({a,b}); }
        for (int i = 1; i <= n; i++) cin >> money[i];
        int s, p; cin >> s >> p;
        vector<bool> bar(n + 1, false);
        for (int i = 0; i < p; i++) { int x; cin >> x; bar[x] = true; }
        buildSCC();
        int k = sccs.size();
        vector<int> sum(k, 0);
        vector<bool> hasBar(k, false);
        for (int a = 0; a < k; a++) {
            for (int x : sccs[a]) { sum[a] += money[x]; if (bar[x]) hasBar[a] = true; }
        }
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) g[a].push_back(b);
        }
        for (int a = 0; a < k; a++) { sort(g[a].begin(), g[a].end()); g[a].erase(unique(g[a].begin(), g[a].end()), g[a].end()); }
        for (int a = 0; a < k; a++) for (int b : g[a]) indeg[b]++;
        queue<int> q;
        vector<int> dp(k, INT_MIN / 2);
        int st = belong[s];
        dp[st] = sum[st];
        for (int i = 0; i < k; i++) if (indeg[i] == 0) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (dp[u] != INT_MIN / 2) dp[v] = max(dp[v], dp[u] + sum[v]);
                if (--indeg[v] == 0) q.push(v);
            }
        }
        int ans = 0;
        for (int i = 0; i < k; i++) if (hasBar[i] && dp[i] != INT_MIN / 2) ans = max(ans, dp[i]);
        cout << ans << "\n";
    }
    else if (which == 4) {
        int m; cin >> n >> m;
        vector<int> val(n + 1);
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 1; i <= n; i++) cin >> val[i];
        for (int i = 0; i < m; i++) {
            int a, b, op; cin >> a >> b >> op;
            R[a][b] = true; es.push_back({a, b});
            if (op == 2) { R[b][a] = true; es.push_back({b, a}); }
        }
        buildSCC();
        int k = sccs.size();
        vector<int> mn(k, INT_MAX), mx(k, INT_MIN);
        for (int a = 0; a < k; a++) for (int x : sccs[a]) { mn[a] = min(mn[a], val[x]); mx[a] = max(mx[a], val[x]); }
        int s = belong[1], t = belong[n];
        if (t < 0 || s < 0) { cout << 0 << "\n"; return 0; }
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) g[a].push_back(b);
        }
        for (int a = 0; a < k; a++) { sort(g[a].begin(), g[a].end()); g[a].erase(unique(g[a].begin(), g[a].end()), g[a].end()); }
        for (int a = 0; a < k; a++) for (int b : g[a]) indeg[b]++;
        queue<int> q;
        vector<int> premin(k, INT_MAX), dp(k, INT_MIN / 2);
        premin[s] = mn[s]; dp[s] = mx[s] - mn[s];
        for (int i = 0; i < k; i++) if (indeg[i] == 0) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (premin[u] != INT_MAX) premin[v] = min(premin[v], min(premin[u], mn[v]));
                if (dp[u] != INT_MIN / 2) dp[v] = max(dp[v], max(dp[u], mx[v] - premin[v]));
                if (--indeg[v] == 0) q.push(v);
            }
        }
        cout << (dp[t] == INT_MIN / 2 ? 0 : max(0, dp[t])) << "\n";
    }
    else if (which == 5) {
        int m, x; cin >> n >> m >> x;
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 0; i < m; i++) { int a, b; cin >> a >> b; R[a][b] = true; es.push_back({a,b}); }
        buildSCC();
        int k = sccs.size();
        vector<int> siz(k);
        for (int a = 0; a < k; a++) siz[a] = sccs[a].size();
        // 重边去重（同分量对只留一条）
        set<pair<int,int>> seen;
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) seen.insert({a, b});
        }
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& p : seen) { g[p.first].push_back(p.second); indeg[p.second]++; }
        queue<int> q;
        vector<int> dpSum(k, 0), dpCnt(k, 0);
        for (int i = 0; i < k; i++) if (indeg[i] == 0) { q.push(i); dpSum[i] = siz[i]; dpCnt[i] = 1 % x; }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (dpSum[u] + siz[v] > dpSum[v]) {
                    dpSum[v] = dpSum[u] + siz[v];
                    dpCnt[v] = dpCnt[u];
                } else if (dpSum[u] + siz[v] == dpSum[v]) {
                    dpCnt[v] = (dpCnt[v] + dpCnt[u]) % x;
                }
                if (--indeg[v] == 0) q.push(v);
            }
        }
        int ans1 = 0, ans2 = 0;
        for (int i = 0; i < k; i++) {
            if (dpSum[i] > ans1) { ans1 = dpSum[i]; ans2 = dpCnt[i]; }
            else if (dpSum[i] == ans1) ans2 = (ans2 + dpCnt[i]) % x;
        }
        cout << ans1 << "\n" << ans2 % x << "\n";
    }
    else if (which == 6) {
        int m; cin >> n >> m;
        vector<int> a(m + 1), b(m + 1), w(m + 1);
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 1; i <= m; i++) { cin >> a[i] >> b[i] >> w[i]; R[a[i]][b[i]] = true; }
        buildSCC();
        int s = belong[1], t = belong[n];
        if (s < 0 || t < 0) { cout << -1 << "\n"; return 0; }
        int k = sccs.size();
        vector<vector<pair<int,int>>> g(k);
        for (int i = 1; i <= m; i++) {
            int s1 = belong[a[i]], s2 = belong[b[i]];
            if (s1 != s2) g[s1].push_back({s2, w[i]});
        }
        // dijkstra
        vector<int> dist(k, INT_MAX / 2);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        dist[s] = 0; pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (auto& e : g[u]) {
                int v = e.first, ww = e.second;
                if (dist[u] + ww < dist[v]) { dist[v] = dist[u] + ww; pq.push({dist[v], v}); }
            }
        }
        cout << (dist[t] == INT_MAX / 2 ? -1 : dist[t]) << "\n";
    }
    else if (which == 7) {
        int m; cin >> n >> m;
        vector<int> w(n + 1), v(n + 1), depend(n + 1);
        for (int i = 1; i <= n; i++) cin >> w[i];
        for (int i = 1; i <= n; i++) cin >> v[i];
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 1; i <= n; i++) {
            cin >> depend[i];
            if (depend[i] > 0) { R[depend[i]][i] = true; es.push_back({depend[i], i}); }
        }
        buildSCC();
        int k = sccs.size();
        vector<int> W(k, 0), V(k, 0);
        for (int a = 0; a < k; a++) for (int x : sccs[a]) { W[a] += w[x]; V[a] += v[x]; }
        // 依赖森林（分量 A 依赖 B：B->A 边在 R 中；g[父]→[子]）
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) { g[a].push_back(b); indeg[b]++; }
        }
        // 树上背包（森林 + 虚根），每分量必选自身后才能取子树
        // dp[u][j]: u 子树内、容量 j、且 u 必选时的最大价值
        vector<vector<int>> dp(k, vector<int>(m + 1, -1));
        vector<int> visited(k, 0);
        function<void(int)> dfs = [&](int u) {
            visited[u] = 1;
            vector<int> cur(m + 1, -1);
            if (W[u] <= m) cur[W[u]] = V[u];
            for (int c : g[u]) {
                dfs(c);
                vector<int> nxt = cur;
                for (int j = 0; j <= m; j++) {
                    if (cur[j] < 0) continue;
                    for (int cj = 0; cj + j <= m; cj++) {
                        if (dp[c][cj] < 0) continue;
                        nxt[j + cj] = max(nxt[j + cj], cur[j] + dp[c][cj]);
                    }
                }
                cur = nxt;
            }
            dp[u] = cur;
        };
        int ans = 0;
        // 跨树 0-1 合并（各根树共享总容量）
        vector<int> glob(m + 1, -1);
        glob[0] = 0;
        for (int u = 0; u < k; u++) {
            if (indeg[u] == 0 && !visited[u]) {
                dfs(u);
                vector<int> nxt = glob;
                for (int j = 0; j <= m; j++) {
                    if (glob[j] < 0) continue;
                    for (int cj = 0; cj + j <= m; cj++) {
                        if (dp[u][cj] < 0) continue;
                        nxt[j + cj] = max(nxt[j + cj], glob[j] + dp[u][cj]);
                    }
                }
                glob = nxt;
            }
        }
        for (int j = 0; j <= m; j++) ans = max(ans, glob[j]);
        cout << ans << "\n";
    }
    else if (which == 8) {
        int k; cin >> n >> k;
        vector<tuple<int,int,int>> edges; // a b w
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 0; i < k; i++) {
            int op, u, v; cin >> op >> u >> v;
            if (op == 1) { R[u][v] = R[v][u] = true; edges.push_back({u,v,0}); edges.push_back({v,u,0}); }
            else if (op == 2) { R[u][v] = true; edges.push_back({u,v,1}); }
            else if (op == 3) { R[v][u] = true; edges.push_back({v,u,0}); }
            else if (op == 4) { R[v][u] = true; edges.push_back({v,u,1}); }
            else { R[u][v] = true; edges.push_back({u,v,0}); }
        }
        buildSCC();
        int kk = sccs.size();
        // 分量内正权边 → 正环
        for (auto& [u, v, w] : edges) {
            if (belong[u] == belong[v] && w > 0) { cout << -1 << "\n"; return 0; }
        }
        vector<vector<pair<int,int>>> g(kk);
        vector<int> indeg(kk, 0);
        for (auto& [u, v, w] : edges) {
            int a = belong[u], b = belong[v];
            if (a != b) { g[a].push_back({b, w}); }
        }
        // Kahn 最长路：dp=1 起，按拓扑序 max(dp[v], dp[u]+w)
        for (int a = 0; a < kk; a++) for (auto& e : g[a]) indeg[e.first]++;
        queue<int> q;
        vector<ll> dp(kk, 0);
        for (int i = 0; i < kk; i++) if (indeg[i] == 0) { q.push(i); dp[i] = 1; }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : g[u]) {
                dp[e.first] = max(dp[e.first], dp[u] + e.second);
                if (--indeg[e.first] == 0) q.push(e.first);
            }
        }
        ll ans = 0;
        for (int i = 0; i < kk; i++) ans += dp[i] * (ll)sccs[i].size();
        cout << ans << "\n";
    }
    else if (which == 9) {
        int m; cin >> n >> m;
        vector<pair<int,int>> es;
        R.assign(n + 1, vector<bool>(n + 1, false));
        for (int i = 1; i <= n; i++) R[i][i] = true;
        for (int i = 0; i < m; i++) { int a, b; cin >> a >> b; R[a][b] = true; es.push_back({a,b}); }
        buildSCC();
        int k = sccs.size();
        // 缩点直接边去重算入度
        set<pair<int,int>> seen;
        for (auto& e : es) {
            int a = belong[e.first], b = belong[e.second];
            if (a != b) seen.insert({a, b});
        }
        vector<vector<int>> g(k);
        vector<int> indeg(k, 0);
        for (auto& p : seen) { g[p.first].push_back(p.second); indeg[p.second]++; }
        int inZero = 0;
        for (int i = 0; i < k; i++) if (indeg[i] == 0) inZero++;
        for (int i = 0; i < k; i++) {
            if (indeg[i] > 0 || (int)sccs[i].size() > 1) continue;
            if (g[i].empty()) { inZero--; break; }
            bool ok = true;
            for (int v : g[i]) if (indeg[v] == 1) { ok = false; break; }
            if (ok) { inZero--; break; }
        }
        double ans = 1.0 - (double)inZero / (double)n;
        printf("%.6f\n", ans);
    }
    return 0;
}
