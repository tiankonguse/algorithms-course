#include <bits/stdc++.h>

using namespace std;

struct Data {
    int i, a, b, c;
};

bool CmpABC(Data x, Data y) {
    if (x.a != y.a) {
        return x.a < y.a;
    }
    if (x.b != y.b) {
        return x.b < y.b;
    }
    return x.c < y.c;
}

bool CmpB(Data x, Data y) {
    return x.b < y.b;
}

const int MAXN = 101;
const int MAXM = 1001;
const int MAXV = 10001;
const int MAXT = 100001;
const int MAXE = 100001;
int n, m, maxc, cntt;
int a[MAXN];
int b[MAXN];
int c[MAXN];

Data arr[MAXN];
int cnta;

int headg[MAXT];
int nextg[MAXE];
int tog[MAXE];
int weightg[MAXE];
int cntg;

int headp[MAXT];
int nextp[MAXM << 1];
int top[MAXM << 1];
int weightp[MAXM << 1];
int cntp;

int tree[MAXV];

void addEdge(int u, int v, int w) {
    nextg[++cntg] = headg[u];
    tog[cntg] = v;
    weightg[cntg] = w;
    headg[u] = cntg;
}

void addOp(int x, int u, int w) {
    nextp[++cntp] = headp[x];
    top[cntp] = u;
    weightp[cntp] = w;
    headp[x] = cntp;
}

int lowbit(int i) {
    return i & -i;
}

void add(int i, int x) {
    while (i <= maxc) {
        int pre = tree[i];
        int cur = ++cntt;
        if (pre != 0) {
            addEdge(pre, cur, 0);
        }
        addEdge(x, cur, 0);
        tree[i] = cur;
        i += lowbit(i);
    }
}

void rangeToX(int i, int x, int w) {
    while (i > 0) {
        if (tree[i] > 0) {
            addEdge(tree[i], x, w);
        }
        i -= lowbit(i);
    }
}

void clear(int val) {
    while (val <= maxc) {
        tree[val] = 0;
        val += lowbit(val);
    }
}

void merge(int l, int m, int r) {
    int p1, p2;
    for (p1 = l - 1, p2 = m + 1; p2 <= r; p2++) {
        while (p1 + 1 <= m && arr[p1 + 1].b <= arr[p2].b) {
            p1++;
            add(arr[p1].c, arr[p1].i);
        }
        for (int e = headp[arr[p2].i]; e > 0; e = nextp[e]) {
            int u = top[e];
            int w = weightp[e];
            rangeToX(arr[p2].c, u, w);
        }
    }
    for (int i = l; i <= p1; i++) {
        clear(arr[i].c);
    }
    sort(arr + l, arr + r + 1, CmpB);
}

void cdq(int l, int r) {
    if (l == r) {
        return;
    }
    int mid = (l + r) / 2;
    cdq(l, mid);
    cdq(mid + 1, r);
    merge(l, mid, r);
}

void buildGraph() {
    for (int i = 1; i <= n; i++) {
        arr[i] = {i, a[i], b[i], c[i]};
        maxc = max(maxc, c[i]);
    }
    sort(arr + 1, arr + n + 1, CmpABC);
    cntt = n;
    for (int l = 1, r = 1; l <= n; l = ++r) {
        int a = arr[l].a;
        int b = arr[l].b;
        int c = arr[l].c;
        while (r + 1 <= n && a == arr[r + 1].a && b == arr[r + 1].b && c == arr[r + 1].c) {
            r++;
        }
        int x = ++cntt;
        for (int i = l; i <= r; i++) {
            int u = arr[i].i;
            addEdge(u, x, 0);
            for (int e = headp[u]; e > 0; e = nextp[e]) {
                addEdge(x, u, weightp[e]);
                addOp(x, u, weightp[e]);
            }
        }
        arr[++cnta] = {x, a, b, c};
    }
    cdq(1, cnta);
}

const int MAX_2 = 100001;
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

void link(int x, int w) {
    for (int i = 1; i <= n; i++) {
        if (a[i] <= a[x] && b[i] <= b[x] && c[i] <= c[x]) {
            addEdge_2(i, x, w);
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
            for (int e = headg[u]; e > 0; e = nextg[e]) {
                int v = tog[e];
                int w = weightg[e];
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

int random(int v) {
    return rand() % v + 1;
}

void clear() {
    for (int i = 1; i <= cntt; i++) {
        headg[i] = headp[i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        head_2[i] = 0;
    }
    maxc = cnta = cntg = cntp = cntt = cnt_2 = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand((unsigned)time(nullptr));
    cout << "cdq优化建图，三维偏序" << endl;
    cout << "=============" << endl;
    n = 100;
    m = 1000;
    int valMax = 10;
    int weightMax = 10000;
    int round = 20;
    for (int t = 1; t <= round; t++) {
        cout << "第" << t << "轮" << endl;
        cout << "测试开始" << endl;
        for (int i = 1; i <= n; i++) {
            a[i] = random(valMax);
            b[i] = random(valMax);
            c[i] = random(valMax);
        }
        for (int i = 1; i <= m; i++) {
            int x = random(n);
            int w = random(weightMax);
            addOp(x, x, w);
            link(x, w);
        }
        buildGraph();
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
