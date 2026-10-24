#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;
    cin >> n >> m;
    cerr << "n=" << n << " m=" << m << "\n";
    vector<pair<int,int>> es(m);
    cerr << "es ok\n";
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
    cerr << "comps={} -> " << comps({}) << "\n";
    int base = comps({});
    vector<int> cut;
    for (int v = 1; v <= n; v++) {
        vector<int> del(n + 1, 0);
        del[v] = 1;
        if (comps(del) > base) cut.push_back(v);
    }
    cout << cut.size() << "\n";
    for (int v : cut) cout << v << " ";
    cout << "\n";
    return 0;
}
