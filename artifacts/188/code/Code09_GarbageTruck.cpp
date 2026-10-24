#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXM = 2000001;
int n, m;

int head[MAXN];
int nxt[MAXM];
int to[MAXM];
int eid[MAXM];
int cntg;

bool visNode[MAXN];
bool visEdge[MAXM];

int deg[MAXN];
int cur[MAXN];

bool inpath[MAXN];
int path[MAXM];
int cntp;

int ansArr[MAXM];
int ansl[MAXM];
int ansr[MAXM];
int idx;
int cnta;

void addEdge(int u, int v, int id) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    eid[cntg] = id;
    head[u] = cntg;
}

void popCircle(int u) {
    cnta++;
    ansArr[++idx] = u;
    ansl[cnta] = idx;
    for (int pop = path[cntp--]; pop != u; pop = path[cntp--]) {
        inpath[pop] = false;
        ansArr[++idx] = pop;
    }
    inpath[u] = false;
    ansArr[++idx] = u;
    ansr[cnta] = idx;
}

void euler(int u) {
    visNode[u] = true;
    for (int e = cur[u]; e > 0; e = cur[u]) {
        cur[u] = nxt[e];
        if (!visEdge[eid[e]]) {
            visEdge[eid[e]] = true;
            euler(to[e]);
        }
    }
    if (inpath[u]) {
        popCircle(u);
    }
    inpath[u] = true;
    path[++cntp] = u;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1, u, v, s, t; i <= m; i++) {
        cin >> u >> v >> s >> t;
        if (s != t) {
            deg[u]++;
            deg[v]++;
            addEdge(u, v, i);
            addEdge(v, u, i);
        }
    }
    for (int i = 1; i <= n; i++) {
        cur[i] = head[i];
    }
    bool check = true;
    for (int i = 1; i <= n; i++) {
        if ((deg[i] & 1) == 1) {
            check = false;
            break;
        }
    }
    if (!check) {
        cout << "NIE\n";
    } else {
        for (int i = 1; i <= n; i++) {
            if (!visNode[i]) {
                euler(i);
            }
        }
        cout << cnta << "\n";
        for (int i = 1; i <= cnta; i++) {
            cout << (ansr[i] - ansl[i]) << " ";
            for (int j = ansl[i]; j <= ansr[i]; j++) {
                cout << ansArr[j] << " ";
            }
            cout << "\n";
        }
    }
    return 0;
}
