#include <bits/stdc++.h>

using namespace std;

const int MAXN = 500001;
const int MAXP = 1000002;
const int MAXT = 5000001;
const int MAXE = 10000001;
int n, cntt;

string arr[MAXN << 1];

int headg[MAXT];
int nextg[MAXE];
int tog[MAXE];
int cntg;

int tree[MAXP][2];
int fa[MAXP];
int up[MAXP];
int down[MAXP];
int cntp;

int headx[MAXP];
int nextx[MAXP];
int tox[MAXP];
int cntx;

int group[MAXN];
int gsiz;

int dfn[MAXT];
int low[MAXT];
int cntd;

int sta[MAXT];
int top;

int belong[MAXT];
int sccCnt;

void addEdge(int u, int v) {
    nextg[++cntg] = headg[u];
    tog[cntg] = v;
    headg[u] = cntg;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = headg[u]; e > 0; e = nextg[e]) {
        int v = tog[e];
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else {
            if (belong[v] == 0) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }
    if (dfn[u] == low[u]) {
        sccCnt++;
        int pop;
        do {
            pop = sta[top--];
            belong[pop] = sccCnt;
        } while (pop != u);
    }
}

int other(int x) {
    return x <= n ? x + n : x - n;
}

void addGroup(int u, int x) {
    nextx[++cntx] = headx[u];
    tox[cntx] = x;
    headx[u] = cntx;
}

void insert(int x) {
    string str = arr[x];
    int cur = 1;
    for (int si = 0; si < (int)str.length(); si++) {
        int path = str[si] == '0' ? 0 : 1;
        if (tree[cur][path] == 0) {
            tree[cur][path] = ++cntp;
            fa[cntp] = cur;
            up[cntp] = ++cntt;
            down[cntp] = ++cntt;
            addEdge(up[cntp], up[cur]);
            addEdge(down[cur], down[cntp]);
        }
        cur = tree[cur][path];
    }
    addEdge(x, up[fa[cur]]);
    addEdge(up[cur], other(x));
    addEdge(x, down[cur]);
    addEdge(down[fa[cur]], other(x));
    addGroup(cur, x);
}

void groupLink() {
    if (gsiz > 1) {
        cntt++;
        addEdge(cntt, other(group[1]));
        for (int i = 2; i <= gsiz; i++) {
            cntt++;
            addEdge(cntt, other(group[i]));
            addEdge(group[i], cntt - 1);
            addEdge(cntt, cntt - 1);
        }
        cntt++;
        addEdge(cntt, other(group[gsiz]));
        for (int i = gsiz - 1; i >= 1; i--) {
            cntt++;
            addEdge(cntt, other(group[i]));
            addEdge(group[i], cntt - 1);
            addEdge(cntt, cntt - 1);
        }
    }
}

void buildGraph() {
    cntt = n << 1;
    cntp = 1;
    up[1] = ++cntt;
    down[1] = ++cntt;
    for (int x = 1; x <= (n << 1); x++) {
        insert(x);
    }
    for (int u = 1; u <= cntp; u++) {
        gsiz = 0;
        for (int e = headx[u]; e > 0; e = nextx[e]) {
            group[++gsiz] = tox[e];
        }
        groupLink();
    }
}

void addString(int i, string s) {
    int mark = -1;
    for (int si = 0; si < (int)s.length(); si++) {
        if (s[si] == '?') {
            mark = si;
            break;
        }
    }
    if (mark == -1) {
        if (s[0] == '0') {
            addEdge(i + n, i);
        } else {
            addEdge(i, i + n);
        }
        mark = 0;
    }
    s[mark] = '0';
    arr[i] = s;
    s[mark] = '1';
    arr[i + n] = s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    string s;
    for (int i = 1; i <= n; i++) {
        cin >> s;
        addString(i, s);
    }
    buildGraph();
    for (int i = 1; i <= (n << 1); i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1; i <= n; i++) {
        if (belong[i] == belong[i + n]) {
            check = false;
            break;
        }
    }
    if (check) {
        cout << "YES" << "\n";
        for (int i = 1; i <= n; i++) {
            if (belong[i] < belong[i + n]) {
                cout << arr[i] << "\n";
            } else {
                cout << arr[i + n] << "\n";
            }
        }
    } else {
        cout << "NO" << "\n";
    }
    return 0;
}
