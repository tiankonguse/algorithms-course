#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAXN = 200001;
int n;

int head[MAXN];
int nxt[MAXN << 1];
int to[MAXN << 1];
int weight[MAXN << 1];
int cntg;

int dfn[MAXN];
int cntd;

int fromNode[MAXN];
int fromWeight[MAXN];
bool cycle[MAXN];
int arr[MAXN];
int val[MAXN];
int cnta;

ll dist[MAXN];
ll diameter;

ll height[MAXN];

ll preMax[MAXN];
ll preDiameter[MAXN];

ll sufMax[MAXN];
ll sufDiameter[MAXN];

void addEdge(int u, int v, int w) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    weight[cntg] = w;
    head[u] = cntg;
}

void dfs(int u) {
    dfn[u] = ++cntd;
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        if (dfn[v] == 0) {
            fromNode[v] = u;
            fromWeight[v] = weight[e];
            dfs(v);
        } else if (dfn[u] < dfn[v]) {
            cycle[u] = true;
            arr[++cnta] = u;
            val[cnta] = weight[e];
            for (int i = v; i != u; i = fromNode[i]) {
                cycle[i] = true;
                arr[++cnta] = i;
                val[cnta] = fromWeight[i];
            }
        }
    }
}

void dpOnTree(int u, int fa) {
    for (int e = head[u]; e > 0; e = nxt[e]) {
        int v = to[e];
        int w = weight[e];
        if (v != fa && !cycle[v]) {
            dpOnTree(v, u);
            diameter = max(diameter, dist[u] + dist[v] + w);
            dist[u] = max(dist[u], dist[v] + w);
        }
    }
}

ll dpOnCycle() {
    ll best = 0;
    ll sum = 0;
    for (int i = 1; i <= cnta; i++) {
        preMax[i] = max(preMax[i - 1], height[i] + sum);
        preDiameter[i] = max(preDiameter[i - 1], sum + height[i] + best);
        best = max(best, height[i] - sum);
        sum += val[i];
    }
    sufMax[cnta] = height[cnta] + val[cnta];
    sufDiameter[cnta] = height[cnta];
    best = height[cnta] - val[cnta];
    sum = val[cnta] + val[cnta - 1];
    for (int i = cnta - 1; i >= 1; i--) {
        sufMax[i] = max(sufMax[i + 1], height[i] + sum);
        sufDiameter[i] = max(sufDiameter[i + 1], sum + height[i] + best);
        best = max(best, height[i] - sum);
        sum += val[i - 1];
    }
    ll ans = LLONG_MAX;
    for (int i = 1; i < cnta; i++) {
        ans = min(ans, max(preMax[i] + sufMax[i + 1], max(preDiameter[i], sufDiameter[i + 1])));
    }
    ans = min(ans, preDiameter[cnta]);
    return ans;
}

ll compute() {
    dfs(1);
    ll ans1 = 0;
    for (int i = 1; i <= cnta; i++) {
        diameter = 0;
        dpOnTree(arr[i], 0);
        ans1 = max(ans1, diameter);
        height[i] = dist[arr[i]];
    }
    ll ans2 = dpOnCycle();
    return max(ans1, ans2);
}

int main() {
    scanf("%d", &n);
    for (int i = 1, u, v, w; i <= n; i++) {
        scanf("%d%d%d", &u, &v, &w);
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    ll ans = compute();

    // 王国道路，CF835F，使用如下打印
    printf("%lld\n", ans);

    // 快餐店，洛谷P1399，使用如下打印
    // printf("%.1f\n", (double)ans / 2.0);

    return 0;
}
