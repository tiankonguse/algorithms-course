#include <bits/stdc++.h>

using namespace std;

struct Node {
    int idx, jump, time;
};

const int MAXN = 30001;
int n, m;
int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int cnt;

deque<Node> que;
bitset<MAXN> vis[MAXN];

void add(int idx, int jump) {
    nxt[++cnt] = head[idx];
    to[cnt] = jump;
    head[idx] = cnt;
}

void trigger(int idx, int time) {
    for (int e = head[idx], jump; e; e = nxt[e]) {
        jump = to[e];
        if (!vis[idx].test(jump)) {
            vis[idx].set(jump);
            que.push_back({idx, jump, time});
        }
    }
    head[idx] = 0;
}

void extend(int idx, int jump, int time) {
    trigger(idx, time);
    if (!vis[idx].test(jump)) {
        vis[idx].set(jump);
        que.push_back({idx, jump, time});
    }
}

int bfs(int s, int t) {
    if (s == t) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        vis[i].reset();
    }
    que.clear();
    trigger(s, 0);
    while (!que.empty()) {
        Node cur = que.front();
        que.pop_front();
        int idx = cur.idx;
        int jump = cur.jump;
        int time = cur.time;
        if (idx - jump == t || idx + jump == t) {
            return time + 1;
        }
        if (idx - jump >= 0) {
            extend(idx - jump, jump, time + 1);
        }
        if (idx + jump < n) {
            extend(idx + jump, jump, time + 1);
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    int s, sjump, t, tjump;
    cin >> s >> sjump >> t >> tjump;
    add(s, sjump);
    add(t, tjump);
    for (int i = 2, idx, jump; i < m; i++) {
        cin >> idx >> jump;
        add(idx, jump);
    }
    cout << bfs(s, t) << '\n';
    return 0;
}
