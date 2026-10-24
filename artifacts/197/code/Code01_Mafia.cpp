#include <bits/stdc++.h>

using namespace std;

struct Node {
    int eid, nid, color;
};

bool NodeCmp(Node a, Node b) {
    if (a.nid != b.nid) {
        return a.nid < b.nid;
    }
    return a.color < b.color;
}

const int MAXM = 50001;
const int MAXT = 500001;
const int MAXE = 1000001;
int n, m, k, maxt, cntt;

Node arr[MAXM << 1];
int destroyTime[MAXM];

int head[MAXT];
int nxt[MAXE];
int to[MAXE];
int cntg;

int group[MAXM];
int gsiz;

int dfn[MAXT];
int low[MAXT];
int cntd;

int sta[MAXT];
int top;

int belong[MAXT];
int sccCnt;

int ansTime;
int ansSize;
int ansArr[MAXM];

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
    return i <= m ? i + m : i - m;
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

void buildGraph(int limit) {
    cntt = m << 1;
    for (int i = 1; i <= m; i++) {
        if (destroyTime[i] > limit) {
            addEdge(i, i + m);
        }
    }
    for (int l = 1, r = 1; l <= k; l = ++r) {
        int curx = arr[l].nid;
        while (r + 1 <= k && curx == arr[r + 1].nid) {
            r++;
        }
        gsiz = 0;
        for (int i = l; i <= r; i++) {
            group[++gsiz] = arr[i].eid;
        }
        groupLink();
    }
    for (int l = 1, r = 1; l <= k; l = ++r) {
        int curx = arr[l].nid;
        int curc = arr[l].color;
        while (r + 1 <= k && curx == arr[r + 1].nid && curc == arr[r + 1].color) {
            r++;
        }
        gsiz = 0;
        for (int i = l; i <= r; i++) {
            group[++gsiz] = other(arr[i].eid);
        }
        groupLink();
    }
}

void clear() {
    for (int i = 1; i <= cntt; i++) {
        head[i] = dfn[i] = belong[i] = 0;
    }
    cntt = cntg = cntd = top = sccCnt = 0;
}

bool getAns(int limit) {
    buildGraph(limit);
    for (int i = 1; i <= (m << 1); i++) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    bool check = true;
    for (int i = 1; i <= m; i++) {
        if (belong[i] == belong[i + m]) {
            check = false;
            break;
        }
    }
    if (check) {
        ansTime = ansSize = 0;
        for (int i = 1; i <= m; i++) {
            if (belong[i] < belong[i + m]) {
                ansTime = max(ansTime, destroyTime[i]);
                ansArr[++ansSize] = i;
            }
        }
    }
    clear();
    return check;
}

bool compute() {
    sort(arr + 1, arr + k + 1, NodeCmp);
    int l = 1, r = maxt, mid;
    bool hasAns = false;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (getAns(mid)) {
            hasAns = true;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return hasAns;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    maxt = 0;
    for (int i = 1, x, y, c, t; i <= m; i++) {
        cin >> x >> y >> c >> t;
        arr[++k].eid = i;
        arr[k].nid = x;
        arr[k].color = c;
        arr[++k].eid = i;
        arr[k].nid = y;
        arr[k].color = c;
        destroyTime[i] = t;
        maxt = max(maxt, t);
    }
    bool check = compute();
    if (check) {
        cout << "Yes" << "\n";
        cout << ansTime << " " << ansSize << "\n";
        for (int i = 1; i <= ansSize; i++) {
            cout << ansArr[i] << " ";
        }
    } else {
        cout << "No" << "\n";
    }
    return 0;
}
