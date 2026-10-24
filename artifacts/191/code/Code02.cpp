class Solution {
public:
    static const int MAXN = 100001;
    static const int MAXM = 100001;
    int n, m;

    int a[MAXM];
    int b[MAXM];

    int head[MAXN];
    int nxt[MAXM << 1];
    int to[MAXM << 1];
    int cntg;

    int dfn[MAXN];
    int low[MAXN];
    int cntd;

    bool cutEdge[MAXM];

    void prepare() {
        cntg = 1;
        cntd = 0;
        for (int i = 1; i <= n; i++) {
            head[i] = dfn[i] = low[i] = 0;
        }
        for (int i = 1; i <= m; i++) {
            cutEdge[i] = false;
        }
    }

    void addEdge(int u, int v) {
        nxt[++cntg] = head[u];
        to[cntg] = v;
        head[u] = cntg;
    }

    void tarjan(int u, int preEdge) {
        dfn[u] = low[u] = ++cntd;
        for (int e = head[u]; e > 0; e = nxt[e]) {
            if ((e ^ 1) == preEdge) {
                continue;
            }
            int v = to[e];
            if (dfn[v] == 0) {
                tarjan(v, e);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    cutEdge[e >> 1] = true;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    vector<vector<int>> criticalConnections(int nodeCnt, vector<vector<int>>& connections) {
        n = nodeCnt;
        m = (int)connections.size();
        prepare();
        for (int i = 1; i <= m; i++) {
            a[i] = connections[i - 1][0] + 1;
            b[i] = connections[i - 1][1] + 1;
            addEdge(a[i], b[i]);
            addEdge(b[i], a[i]);
        }
        tarjan(1, 0);
        vector<vector<int>> ans;
        for (int i = 1; i <= m; i++) {
            if (cutEdge[i]) {
                ans.push_back({a[i] - 1, b[i] - 1});
            }
        }
        return ans;
    }
};