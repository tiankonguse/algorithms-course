#include <bits/stdc++.h>
using namespace std;
// 最小数位和 AT_arc084_b：01bfs 同余图，补0权0放队首、加1权1放队尾
int k;
int main() {
    cin >> k;
    vector<bool> vis(k, false);
    deque<pair<int, int>> dq;
    dq.push_back({1, 1});
    while (!dq.empty()) {
        auto cur = dq.front(); dq.pop_front();
        int mod = cur.first, cost = cur.second;
        if (vis[mod]) continue;
        vis[mod] = true;
        if (mod == 0) { cout << cost << endl; return 0; }
        int f = (mod * 10) % k;
        dq.push_front({f, cost});
        int b = (mod + 1) % k;
        dq.push_back({b, cost + 1});
    }
    return 0;
}
