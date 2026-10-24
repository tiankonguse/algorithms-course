#include <bits/stdc++.h>

using namespace std;

struct Set {
    priority_queue<int> addHeap;
    priority_queue<int> delHeap;

    void clean() {
        while (!delHeap.empty() && !addHeap.empty() && delHeap.top() == addHeap.top()) {
            addHeap.pop();
            delHeap.pop();
        }
    }

    int popHead() {
        int ans = addHeap.top();
        addHeap.pop();
        clean();
        return ans;
    }

    int size() {
        return (int)addHeap.size() - (int)delHeap.size();
    }

    void add(int v) {
        addHeap.push(v);
    }

    void del(int v) {
        delHeap.push(v);
        clean();
    }

    int first() {
        return addHeap.top();
    }

    int second() {
        int a = popHead();
        int b = first();
        add(a);
        return b;
    }
};

const int MAXN = 100001;
int n, m;
bool black[MAXN];

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int cntg;

int fa[MAXN];
int dep[MAXN];
int siz[MAXN];
int son[MAXN];
int top[MAXN];

bool vis[MAXN];
int centfa[MAXN];

Set distFa[MAXN];
Set sonMax[MAXN];
Set maxDist;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dfs1(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f] + 1;
    siz[u] = 1;
    for (int e = head[u], v; e; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            dfs1(v, u);
        }
    }
    for (int e = head[u], v; e; e = nxt[e]) {
        v = to[e];
        if (v != f) {
            siz[u] += siz[v];
            if (son[u] == 0 || siz[son[u]] < siz[v]) {
                son[u] = v;
            }
        }
    }
}

void dfs2(int u, int t) {
    top[u] = t;
    if (son[u] == 0) {
        return;
    }
    dfs2(son[u], t);
    for (int e = head[u], v; e; e = nxt[e]) {
        v = to[e];
        if (v != fa[u] && v != son[u]) {
            dfs2(v, v);
        }
    }
}

void getSize(int u, int f) {
    siz[u] = 1;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (v != f && !vis[v]) {
            getSize(v, u);
            siz[u] += siz[v];
        }
    }
}

int getLca(int a, int b) {
    while (top[a] != top[b]) {
        if (dep[top[a]] <= dep[top[b]]) {
            b = fa[top[b]];
        } else {
            a = fa[top[a]];
        }
    }
    return dep[a] <= dep[b] ? a : b;
}

int getDist(int x, int y) {
    return dep[x] + dep[y] - (dep[getLca(x, y)] << 1);
}

int getCentroid(int u, int f) {
    getSize(u, f);
    int half = siz[u] >> 1;
    bool find = false;
    while (!find) {
        find = true;
        for (int e = head[u]; e; e = nxt[e]) {
            int v = to[e];
            if (v != f && !vis[v] && siz[v] > half) {
                f = u;
                u = v;
                find = false;
                break;
            }
        }
    }
    return u;
}

void centroidTree(int u, int f) {
    centfa[u] = f;
    vis[u] = true;
    for (int e = head[u]; e; e = nxt[e]) {
        int v = to[e];
        if (!vis[v]) {
            centroidTree(getCentroid(v, u), u);
        }
    }
}

void addAns(int x) {
    if (sonMax[x].size() >= 2) {
        maxDist.add(sonMax[x].first() + sonMax[x].second());
    }
}

void delAns(int x) {
    if (sonMax[x].size() >= 2) {
        maxDist.del(sonMax[x].first() + sonMax[x].second());
    }
}

void on(int x) {
    delAns(x);
    sonMax[x].del(0);
    addAns(x);
    for (int u = x, f = centfa[u]; f > 0; u = f, f = centfa[u]) {
        delAns(f);
        sonMax[f].del(distFa[u].first());
        distFa[u].del(getDist(x, f));
        if (distFa[u].size() > 0) {
            sonMax[f].add(distFa[u].first());
        }
        addAns(f);
    }
}

void off(int x) {
    delAns(x);
    sonMax[x].add(0);
    addAns(x);
    for (int u = x, f = centfa[u]; f > 0; u = f, f = centfa[u]) {
        delAns(f);
        if (distFa[u].size() > 0) {
            sonMax[f].del(distFa[u].first());
        }
        distFa[u].add(getDist(x, f));
        sonMax[f].add(distFa[u].first());
        addAns(f);
    }
}

void prepare() {
    for (int i = 1; i <= n; i++) {
        black[i] = true;
    }
    for (int i = 1; i <= n; i++) {
        for (int u = i, f = centfa[u]; f > 0; u = f, f = centfa[u]) {
            distFa[u].add(getDist(i, f));
        }
    }
    for (int i = 1; i <= n; i++) {
        sonMax[i].add(0);
        if (centfa[i] > 0) {
            sonMax[centfa[i]].add(distFa[i].first());
        }
    }
    for (int i = 1; i <= n; i++) {
        addAns(i);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs1(1, 0);
    dfs2(1, 1);
    centroidTree(getCentroid(1, 0), 0);
    prepare();
    cin >> m;
    int blackCnt = n;
    char op;
    for (int i = 1, x; i <= m; i++) {
        cin >> op;
        if (op == 'C') {
            cin >> x;
            black[x] = !black[x];
            if (black[x]) {
                off(x);
                blackCnt++;
            } else {
                on(x);
                blackCnt--;
            }
        } else {
            if (blackCnt <= 1) {
                cout << (blackCnt - 1) << '\n';
            } else {
                cout << maxDist.first() << '\n';
            }
        }
    }
    return 0;
}
