// 题4 词链暴力：全排列找字典序最小词链（'.' 连接）或 ***
// 题5 字母对暴力：DFS 找字典序最小字符串 或 No Solution
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (argv[1][0] == '4') {
        int n; cin >> n;
        vector<string> w(n);
        for (int i = 0; i < n; i++) cin >> w[i];
        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        string best = "~";
        do {
            bool ok = true;
            for (int i = 0; i + 1 < n; i++)
                if (w[p[i]].back() != w[p[i+1]][0]) { ok = false; break; }
            if (!ok) continue;
            string s = w[p[0]];
            for (int i = 1; i < n; i++) s += "." + w[p[i]];
            if (s < best) best = s;
        } while (next_permutation(p.begin(), p.end()));
        if (best == "~") cout << "***\n";
        else cout << best << "\n";
    } else {
        int n; cin >> n;
        vector<pair<char,char>> ps(n);
        set<char> cs;
        for (int i = 0; i < n; i++) {
            cin >> ps[i].first >> ps[i].second;
            cs.insert(ps[i].first); cs.insert(ps[i].second);
        }
        vector<char> v(cs.begin(), cs.end());
        int K = v.size();
        // 邻接矩阵（无向）
        vector<vector<int>> adj(K, vector<int>(K, 0));
        auto id = [&](char c) { return int(lower_bound(v.begin(), v.end(), c) - v.begin()); };
        for (auto& pr : ps) {
            int a = id(pr.first), b = id(pr.second);
            adj[a][b]++; adj[b][a]++;
        }
        string best;
        vector<int> cnt(K);
        function<void(int,int,string&,vector<int>&)> dfs = [&](int u, int used, string& cur, vector<int>& cnt) {
            if (used == n) { if (best.empty() || cur < best) best = cur; return; }
            for (int x = 0; x < K; x++) if (adj[u][x] > 0) {
                adj[u][x]--; adj[x][u]--;
                cur.push_back(v[x]);
                dfs(x, used + 1, cur, cnt);
                cur.pop_back();
                adj[u][x]++; adj[x][u]++;
            }
        };
        for (int s = 0; s < K; s++) {
            string cur(1, v[s]);
            dfs(s, 0, cur, cnt);
        }
        if (best.empty()) cout << "No Solution\n";
        else cout << best << "\n";
    }
    return 0;
}
