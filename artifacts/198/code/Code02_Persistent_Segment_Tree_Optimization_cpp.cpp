#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1001;
const int MAXV = 1001;
const int MAXT = 10001;
const int MAXE = 10001;
int n, v, cntt;

bool inBag[MAXN];

int head[MAXT];
int nxt[MAXE];
int to[MAXE];
int weight[MAXE];
int cntg;

int rootOut[MAXN];
int rootIn[MAXN];
int ls[MAXT];
int rs[MAXT];
int curVersion;

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
    head[u] = cntg;
}

int buildOut(int l, int r) {
    int rt = ++cntt;
    if (l < r) {
        int mid = (l + r) >> 1;
        ls[rt] = buildOut(l, mid);
        rs[rt] = buildOut(mid + 1, r);
        addEdge(ls[rt], rt, 0);
        addEdge(rs[rt], rt, 0);
    }
    return rt;
}

int buildIn(int l, int r) {
    int rt = ++cntt;
    if (l < r) {
        int mid = (l + r) >> 1;
        ls[rt] = buildIn(l, mid);
        rs[rt] = buildIn(mid + 1, r);
        addEdge(rt, ls[rt], 0);
        addEdge(rt, rs[rt], 0);
    }
    return rt;
}

int addOut(int jobx, int jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(i, rt, 0);
    if (l == r) {
        addEdge(jobx, rt, 0);
    } else {
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            ls[rt] = addOut(jobx, jobv, l, mid, ls[rt]);
            addEdge(ls[rt], rt, 0);
        } else {
            rs[rt] = addOut(jobx, jobv, mid + 1, r, rs[rt]);
            addEdge(rs[rt], rt, 0);
        }
    }
    return rt;
}

int addIn(int jobx, int jobv, int l, int r, int i) {
    int rt = ++cntt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    addEdge(rt, i, 0);
    if (l == r) {
        addEdge(rt, jobx, 0);
    } else {
        int mid = (l + r) >> 1;
        if (jobv <= mid) {
            ls[rt] = addIn(jobx, jobv, l, mid, ls[rt]);
            addEdge(rt, ls[rt], 0);
        } else {
            rs[rt] = addIn(jobx, jobv, mid + 1, r, rs[rt]);
            addEdge(rt, rs[rt], 0);
        }
    }
    return rt;
}

void add(int x, int xv) {
    if (!inBag[x]) {
        inBag[x] = true;
        curVersion++;
        rootOut[curVersion] = addOut(x, xv, 1, v, rootOut[curVersion - 1]);
        rootIn[curVersion] = addIn(x, xv, 1, v, rootIn[curVersion - 1]);
    }
}

void rangeToX(int jobl, int jobr, int jobx, int jobw, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        addEdge(i, jobx, jobw);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            rangeToX(jobl, jobr, jobx, jobw, l, mid, ls[i]);
        }
        if (jobr > mid) {
            rangeToX(jobl, jobr, jobx, jobw, mid + 1, r, rs[i]);
        }
    }
}

void rangeToX(int l, int r, int x, int w) {
    rangeToX(l, r, x, w, 1, v, rootOut[curVersion]);
}

void xToRange(int jobx, int jobl, int jobr, int jobw, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        addEdge(jobx, i, jobw);
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            xToRange(jobx, jobl, jobr, jobw, l, mid, ls[i]);
        }
        if (jobr > mid) {
            xToRange(jobx, jobl, jobr, jobw, mid + 1, r, rs[i]);
        }
    }
}

void xToRange(int x, int l, int r, int w) {
    xToRange(x, l, r, w, 1, v, rootIn[curVersion]);
}

void rangeToRange(int l1, int r1, int l2, int r2, int w) {
    int x = ++cntt;
    int y = ++cntt;
    rangeToX(l1, r1, x, 0);
    xToRange(y, l2, r2, 0);
    addEdge(x, y, w);
}

const int MAX_2 = 100001;
bool in_bag_2[MAX_2];
int bag_id[MAX_2];
int bag_val[MAX_2];
int bag_siz;

int head_2[MAX_2];
int nxt_2[MAX_2];
int to_2[MAX_2];
int weight_2[MAX_2];
int cnt_2;

void addEdge_2(int u, int v, int w) {
    nxt_2[++cnt_2] = head_2[u];
    to_2[cnt_2] = v;
    weight_2[cnt_2] = w;
    head_2[u] = cnt_2;
}

void add_2(int x, int xv) {
    if (!in_bag_2[x]) {
        in_bag_2[x] = true;
        bag_id[++bag_siz] = x;
        bag_val[bag_siz] = xv;
    }
}

void rangeToX_2(int l, int r, int x, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] >= l && bag_val[i] <= r) {
            addEdge_2(bag_id[i], x, w);
        }
    }
}

void xToRange_2(int x, int l, int r, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] >= l && bag_val[i] <= r) {
            addEdge_2(x, bag_id[i], w);
        }
    }
}

void rangeToRange_2(int l1, int r1, int l2, int r2, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] >= l1 && bag_val[i] <= r1) {
            for (int j = 1; j <= bag_siz; j++) {
                if (bag_val[j] >= l2 && bag_val[j] <= r2) {
                    addEdge_2(bag_id[i], bag_id[j], w);
                }
            }
        }
    }
}

struct Node {
    int u;
    int d;

    bool operator < (const Node &other) const {
        return d > other.d;
    }
};

int INF = 1000000001;
int dist[MAXT];
bool vis[MAXT];
priority_queue<Node> heap;

int dijkstra(int s, int t) {
    for (int i = 1; i <= cntt; i++) {
        dist[i] = INF;
        vis[i] = false;
    }
    while (!heap.empty()) {
        heap.pop();
    }
    dist[s] = 0;
    heap.push({s, 0});
    while (!heap.empty()) {
        Node cur = heap.top();
        heap.pop();
        int u = cur.u;
        int d = cur.d;
        if (u == t) {
            return d;
        }
        if (!vis[u]) {
            vis[u] = true;
            for (int e = head[u]; e > 0; e = nxt[e]) {
                int v = to[e];
                int w = weight[e];
                if (!vis[v] && dist[v] > d + w) {
                    dist[v] = d + w;
                    heap.push({v, dist[v]});
                }
            }
        }
    }
    return -1;
}

int dijkstra2(int s, int t) {
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
        vis[i] = false;
    }
    while (!heap.empty()) {
        heap.pop();
    }
    dist[s] = 0;
    heap.push({s, 0});
    while (!heap.empty()) {
        Node cur = heap.top();
        heap.pop();
        int u = cur.u;
        int d = cur.d;
        if (u == t) {
            return d;
        }
        if (!vis[u]) {
            vis[u] = true;
            for (int e = head_2[u]; e > 0; e = nxt_2[e]) {
                int v = to_2[e];
                int w = weight_2[e];
                if (!vis[v] && dist[v] > d + w) {
                    dist[v] = d + w;
                    heap.push({v, dist[v]});
                }
            }
        }
    }
    return -1;
}

int random(int maxv) {
    return rand() % maxv + 1;
}

void clear() {
    for (int i = 1; i <= cntt; i++) {
        head[i] = 0;
    }
    cntg = 0;
    cntt = 0;
    curVersion = 0;
    for (int i = 1; i <= n; i++) {
        inBag[i] = false;
        in_bag_2[i] = false;
        head_2[i] = 0;
    }
    bag_siz = 0;
    cnt_2 = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand((unsigned)time(nullptr));
    cout << "主席树优化建图" << endl;
    cout << "============" << endl;
    n = 100;
    v = 200;
    int weightMax = 1000;
    int round = 20;
    for (int t = 1; t <= round; t++) {
        cout << "第" << t << "轮" << endl;
        cout << "测试开始" << endl;
        cntt = n;
        rootOut[0] = buildOut(1, v);
        rootIn[0] = buildIn(1, v);
        int opCnt = 200;
        for (int i = 1; i <= opCnt; i++) {
            int op = random(4);
            if (op == 1) {
                int x = random(n);
                int xv = random(v);
                add(x, xv);
                add_2(x, xv);
            } else {
                int a = random(v);
                int b = random(v);
                int l = min(a, b);
                int r = max(a, b);
                a = random(v);
                b = random(v);
                int l2 = min(a, b);
                int r2 = max(a, b);
                int x = random(n);
                int w = random(weightMax);
                if (op == 2) {
                    rangeToX(l, r, x, w);
                    rangeToX_2(l, r, x, w);
                } else if (op == 3) {
                    xToRange(x, l, r, w);
                    xToRange_2(x, l, r, w);
                } else {
                    rangeToRange(l, r, l2, r2, w);
                    rangeToRange_2(l, r, l2, r2, w);
                }
            }
        }
        for (int x = 1; x <= n; x++) {
            for (int y = 1; y <= n; y++) {
                int ans1 = dijkstra(x, y);
                int ans2 = dijkstra2(x, y);
                if (ans1 != ans2) {
                    cout << "出错了!" << endl;
                }
            }
        }
        clear();
        cout << "测试结束" << endl;
        cout << "=======" << endl;
    }
    return 0;
}
