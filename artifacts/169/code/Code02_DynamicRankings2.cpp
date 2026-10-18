#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXE = MAXN << 2;
const int INF = 1000000001;
int n, m;

int arr[MAXN];
int eid[MAXE];
int op[MAXE];
int x[MAXE];
int y[MAXE];
int v[MAXE];
int q[MAXE];
int cnte = 0;
int cntq = 0;

int tree[MAXN];

int lset[MAXE];
int rset[MAXE];

int ans[MAXN];

int lowbit(int i) {
    return i & -i;
}

void add(int i, int v) {
    while (i <= n) {
        tree[i] += v;
        i += lowbit(i);
    }
}

int sum(int i) {
    int ret = 0;
    while (i > 0) {
        ret += tree[i];
        i -= lowbit(i);
    }
    return ret;
}

int query(int l, int r) {
    return sum(r) - sum(l - 1);
}

void compute(int el, int er, int vl, int vr) {
    if (el > er) {
        return;
    }
    if (vl == vr) {
        for (int i = el; i <= er; i++) {
            int id = eid[i];
            if (op[id] == 2) {
                ans[q[id]] = vl;
            }
        }
    } else {
        int mid = (vl + vr) >> 1;
        int lsiz = 0, rsiz = 0;
        for (int i = el; i <= er; i++) {
            int id = eid[i];
            if (op[id] == 1) {
                if (y[id] <= mid) {
                    add(x[id], v[id]);
                    lset[++lsiz] = id;
                } else {
                    rset[++rsiz] = id;
                }
            } else {
                int satisfy = query(x[id], y[id]);
                if (v[id] <= satisfy) {
                    lset[++lsiz] = id;
                } else {
                    v[id] -= satisfy;
                    rset[++rsiz] = id;
                }
            }
        }
        for (int i = 1; i <= lsiz; i++) {
            eid[el + i - 1] = lset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            eid[el + lsiz + i - 1] = rset[i];
        }
        for (int i = 1; i <= lsiz; i++) {
            int id = lset[i];
            if (op[id] == 1 && y[id] <= mid) {
                add(x[id], -v[id]);
            }
        }
        compute(el, el + lsiz - 1, vl, mid);
        compute(el + lsiz, er, mid + 1, vr);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        op[++cnte] = 1;
        x[cnte] = i;
        y[cnte] = arr[i];
        v[cnte] = 1;
    }
    for (int i = 1; i <= m; i++) {
        char type;
        cin >> type;
        if (type == 'C') {
            int a, b;
            cin >> a >> b;
            op[++cnte] = 1;
            x[cnte] = a;
            y[cnte] = arr[a];
            v[cnte] = -1;
            op[++cnte] = 1;
            x[cnte] = a;
            y[cnte] = b;
            v[cnte] = 1;
            arr[a] = b;
        } else {
            op[++cnte] = 2;
            cin >> x[cnte] >> y[cnte] >> v[cnte];
            q[cnte] = ++cntq;
        }
    }
    for (int i = 1; i <= cnte; i++) {
        eid[i] = i;
    }
    compute(1, cnte, 0, INF);
    for (int i = 1; i <= cntq; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
