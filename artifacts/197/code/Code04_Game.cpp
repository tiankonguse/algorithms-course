#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
int n, d, m;

char ban[128];
char first[128];
char second[128];

char groundType[MAXN];
int pos1[MAXN];
char car1[MAXN];
int pos2[MAXN];
char car2[MAXN];

int posx[MAXN];

int head[MAXN];
int nxt[MAXN];
int to[MAXN];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int sccCnt;

char ans[MAXN];

void prepare() {
    ban['a'] = 'A';
    ban['b'] = 'B';
    ban['c'] = 'C';
    first['a'] = 'B';
    first['b'] = 'A';
    first['c'] = 'A';
    second['a'] = 'C';
    second['b'] = 'C';
    second['c'] = 'B';
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void tarjan(int u) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
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

int other(int i) {
    return i <= n ? i + n : i - n;
}

void buildGraph(int stax) {
    for (int bit = 0, idx = 1; bit < d; bit++, idx++) {
        if (((stax >> bit) & 1) == 1) {
            groundType[posx[idx]] = 'a';
        } else {
            groundType[posx[idx]] = 'b';
        }
    }
    for (int i = 1; i <= m; i++) {
        int p1 = pos1[i];
        char g1 = groundType[p1];
        char c1 = car1[i];
        if (ban[g1] == c1) {
            continue;
        }
        int id1 = c1 == first[g1] ? p1 : p1 + n;
        int p2 = pos2[i];
        char g2 = groundType[p2];
        char c2 = car2[i];
        if (ban[g2] == c2) {
            addEdge(id1, other(id1));
        } else {
            int id2 = c2 == first[g2] ? p2 : p2 + n;
            addEdge(id1, id2);
            addEdge(other(id2), other(id1));
        }
    }
}

void clear() {
    for (int i = 1; i <= n << 1; i++) {
        head[i] = dfn[i] = belong[i] = 0;
    }
    cntg = cntd = top = sccCnt = 0;
}

bool compute() {
    for (int i = 1, sizx = 0; i <= n; i++) {
        if (groundType[i] == 'x') {
            posx[++sizx] = i;
        }
    }
    for (int stax = 0; stax < 1 << d; stax++) {
        buildGraph(stax);
        for (int i = 1; i <= n << 1; i++) {
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
            for (int i = 1; i <= n; i++) {
                char ground = groundType[i];
                if (belong[i] < belong[i + n]) {
                    ans[i] = first[ground];
                } else {
                    ans[i] = second[ground];
                }
            }
            return true;
        }
        clear();
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    prepare();
    cin >> n >> d;
    for (int i = 1; i <= n; i++) {
        cin >> groundType[i];
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> pos1[i] >> car1[i] >> pos2[i] >> car2[i];
    }
    bool check = compute();
    if (check) {
        for (int i = 1; i <= n; i++) {
            cout << ans[i];
        }
    } else {
        cout << -1 << "\n";
    }
    return 0;
}
