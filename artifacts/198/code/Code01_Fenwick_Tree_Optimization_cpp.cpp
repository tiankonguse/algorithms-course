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

int outTree[MAXV];
int inTree[MAXV];

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
    head[u] = cntg;
}

int lowbit(int i) {
    return i & -i;
}

void addOut(int x, int xv) {
    while (xv <= v) {
        int preo = outTree[xv];
        int curo = ++cntt;
        if (preo > 0) {
            addEdge(preo, curo, 0);
        }
        addEdge(x, curo, 0);
        outTree[xv] = curo;
        xv += lowbit(xv);
    }
}

void addIn(int x, int xv) {
    while (xv <= v) {
        int prei = inTree[xv];
        int curi = ++cntt;
        if (prei > 0) {
            addEdge(curi, prei, 0);
        }
        addEdge(curi, x, 0);
        inTree[xv] = curi;
        xv += lowbit(xv);
    }
}

void add(int x, int xv) {
    if (!inBag[x]) {
        inBag[x] = true;
        addOut(x, xv);
        addIn(x, xv);
    }
}

void rangeToX(int p, int x, int w) {
    while (p > 0) {
        if (outTree[p] > 0) {
            addEdge(outTree[p], x, w);
        }
        p -= lowbit(p);
    }
}

void xToRange(int x, int p, int w) {
    while (p > 0) {
        if (inTree[p] > 0) {
            addEdge(x, inTree[p], w);
        }
        p -= lowbit(p);
    }
}

void rangeToRange(int p1, int p2, int w) {
    int x = ++cntt;
    int y = ++cntt;
    rangeToX(p1, x, 0);
    xToRange(y, p2, 0);
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

void rangeToX_2(int p, int x, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] <= p) {
            addEdge_2(bag_id[i], x, w);
        }
    }
}

void xToRange_2(int x, int p, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] <= p) {
            addEdge_2(x, bag_id[i], w);
        }
    }
}

void rangeToRange_2(int p1, int p2, int w) {
    for (int i = 1; i <= bag_siz; i++) {
        if (bag_val[i] <= p1) {
            for (int j = 1; j <= bag_siz; j++) {
                if (bag_val[j] <= p2) {
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

const int INF = 1000000001;
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
    for (int i = 1; i <= v; i++) {
        inTree[i] = 0;
        outTree[i] = 0;
    }
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
    cout << "树状数组优化建图" << endl;
    cout << "============" << endl;
    n = 100;
    v = 200;
    int weightMax = 1000;
    int round = 20;
    for (int t = 1; t <= round; t++) {
        cout << "第" << t << "轮" << endl;
        cout << "测试开始" << endl;
        cntt = n;
        int opCnt = 200;
        for (int i = 1; i <= opCnt; i++) {
            int op = random(4);
            if (op == 1) {
                int x = random(n);
                int xv = random(v);
                add(x, xv);
                add_2(x, xv);
            } else {
                int p = random(v);
                int p2 = random(v);
                int x = random(n);
                int w = random(weightMax);
                if (op == 2) {
                    rangeToX(p, x, w);
                    rangeToX_2(p, x, w);
                } else if (op == 3) {
                    xToRange(x, p, w);
                    xToRange_2(x, p, w);
                } else {
                    rangeToRange(p, p2, w);
                    rangeToRange_2(p, p2, w);
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
