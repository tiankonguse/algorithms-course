// 题10 暴力：枚举所有欧拉回路（m<=5），判定是否存在含全部序列的路径
#include <bits/stdc++.h>
using namespace std;
int n, m, t;
vector<pair<int,int>> es;
vector<vector<int>> seqs;
bool found = false;
vector<int> bestPath;

bool contains(const vector<int>& path, const vector<int>& seq) {
    int k = seq.size();
    for (int i = 0; i + k <= (int)path.size(); i++) {
        bool ok = true;
        for (int j = 0; j < k; j++) if (path[i+j] != seq[j]) { ok = false; break; }
        if (ok) return true;
    }
    return false;
}

void dfs(vector<int>& path, int used, int cur) {
    if (found) return;
    if (used == (1 << m) - 1) {
        if (cur != 1) return;
        for (auto& sq : seqs) if (!contains(path, sq)) return;
        found = true; bestPath = path;
        return;
    }
    for (int e = 0; e < m; e++) if (!(used >> e & 1)) {
        if (es[e].first == cur) {
            path.push_back(es[e].second);
            dfs(path, used | (1 << e), es[e].second);
            path.pop_back();
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> m;
    es.resize(m);
    for (int i = 0; i < m; i++) cin >> es[i].first >> es[i].second;
    cin >> t;
    for (int i = 0; i < t; i++) {
        int k; cin >> k;
        vector<int> sq(k);
        for (int j = 0; j < k; j++) cin >> sq[j];
        seqs.push_back(sq);
    }
    vector<int> path{1};
    dfs(path, 0, 1);
    if (!found) cout << "NIE\n";
    else {
        cout << "TAK\n";
        for (int i = 0; i < (int)bestPath.size(); i++) {
            if (i) cout << " ";
            cout << bestPath[i];
        }
        cout << "\n";
    }
    return 0;
}
