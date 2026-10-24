#include <bits/stdc++.h>

using namespace std;

const int MAXN = 50001;
const int MAXM = 1000001;
int n, m;

int head[MAXN];
int nxt[MAXM << 1];
int to[MAXM << 1];
int cntg;

int dfn[MAXN];
int low[MAXN];
int cntd;
int sta[MAXN];
int top;

int dist[MAXN];
int arr[MAXN << 1];
int que[MAXN << 1];
int diameter;

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
}

void dpOnCycle(int u, int v) {
    int siz = 0;
    arr[++siz] = dist[u];
    int pop;
    do {
        pop = sta[top--];
        arr[++siz] = dist[pop];
    } while (pop != v);
    for (int i = 1; i <= siz; i++) {
        arr[i + siz] = arr[i];
    }
    int l = 1, r = 0;
    que[++r] = 1;
    for (int i = 2; i <= siz << 1; i++) {
        while (l <= r && (i - que[l]) * 2 > siz) {
            l++;
        }
        diameter = max(diameter, arr[i] + i + arr[que[l]] - que[l]);
        while (l <= r && arr[que[r]] - que[r] <= arr[i] - i) {
            r--;
        }
        que[++r] = i;
    }
    for (int i = 2; i <= siz; i++) {
        dist[u] = max(dist[u], arr[i] + min(i - 1, siz - (i - 1)));
    }
}

void tarjan(int u, int preEdge) {
    dfn[u] = low[u] = ++cntd;
    sta[++top] = u;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        if ((e ^ 1) == preEdge) {
            continue;
        }
        int v = to[e];
        if (dfn[v] == 0) {
            tarjan(v, e);
            if (low[v] < dfn[u]) {
                low[u] = min(low[u], low[v]);
            } else if (low[v] > dfn[u]) {
                top--;
                diameter = max(diameter, dist[u] + dist[v] + 1);
                dist[u] = max(dist[u], dist[v] + 1);
            } else {
                dpOnCycle(u, v);
            }
        } else if (dfn[v] < dfn[u]) {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    cntg = 1;
    for (int i = 1, k, x, y; i <= m; i++) {
        cin >> k >> x;
        for (int j = 2; j <= k; j++) {
            cin >> y;
            addEdge(x, y);
            addEdge(y, x);
            x = y;
        }
    }
    tarjan(1, 0);
    cout << diameter << "\n";
    return 0;
}
