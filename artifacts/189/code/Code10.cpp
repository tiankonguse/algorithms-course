#include <bits/stdc++.h>

using namespace std;

const int MAXN = 10001;
const int MAXM = 30001;
int n, m;

int a[MAXN];
int b[MAXN];
int cntn;
unordered_map<string, int> nameId;

int head[MAXN];
int nxt[MAXM];
int to[MAXM];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;

int sta[MAXN];
int top;

int belong[MAXN];
int sccCnt;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    string girl, boy;
    for (int i = 1; i <= n; i++) {
        cin >> girl >> boy;
        a[i] = ++cntn;
        b[i] = ++cntn;
        nameId[girl] = a[i];
        nameId[boy] = b[i];
        addEdge(a[i], b[i]);
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> girl >> boy;
        addEdge(nameId[boy], nameId[girl]);
    }
    for (int i = 1; i <= cntn; i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (belong[a[i]] == belong[b[i]]) {
            cout << "Unsafe" << "\n";
        } else {
            cout << "Safe" << "\n";
        }
    }
    return 0;
}
