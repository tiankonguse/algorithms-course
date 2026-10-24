#include <bits/stdc++.h>

using namespace std;

const int MAXN = 20001;
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

int edgeCnt[MAXN];
int cntc;

int cycleCnt[MAXN];

string ans;

void multiply(int x) {
    int carry = 0;
    for (int i = ans.size() - 1; i >= 0; i--) {
        int cur = (ans[i] - '0') * x + carry;
        ans[i] = cur % 10 + '0';
        carry = cur / 10;
    }
    string pre = "";
    while (carry > 0) {
        pre += carry % 10 + '0';
        carry /= 10;
    }
    reverse(pre.begin(), pre.end());
    ans = pre + ans;
}

void addEdge(int u, int v) {
    nxt[++cntg] = head[u];
    to[cntg] = v;
    head[u] = cntg;
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
                cycleCnt[u]++;
            } else if (low[v] > dfn[u]) {
                top--;
            } else {
                cntc++;
                edgeCnt[cntc] = 1;
                int pop;
                do {
                    pop = sta[top--];
                    edgeCnt[cntc]++;
                } while (pop != v);
            }
        } else if (dfn[v] < dfn[u]) {
            low[u] = min(low[u], dfn[v]);
            cycleCnt[u]++;
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
    bool check = true;
    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0 || cycleCnt[i] >= 2) {
            check = false;
            break;
        }
    }
    if (check) {
        ans = "1";
        for (int i = 1; i <= cntc; i++) {
            multiply(edgeCnt[i] + 1);
        }
        cout << ans << "\n";
    } else {
        cout << 0 << "\n";
    }
    return 0;
}
