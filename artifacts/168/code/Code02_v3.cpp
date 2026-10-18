#include <bits/stdc++.h>

using namespace std;

struct Number {
    int x, y, v;
};

bool NumberCmp(Number a, Number b) {
    return a.v < b.v;
}

const int MAXN = 501;
const int MAXQ = 1000001;
int n, q;

Number xyv[MAXN * MAXN];
int cntv = 0;

int qid[MAXQ];
int a[MAXQ];
int b[MAXQ];
int c[MAXQ];
int d[MAXQ];
int k[MAXQ];

int tree[MAXN][MAXN];

int lset[MAXQ];
int rset[MAXQ];

int ans[MAXQ];

int lowbit(int i) {
    return i & -i;
}

void add(int x, int y, int v) {
    for (int i = x; i <= n; i += lowbit(i)) {
        for (int j = y; j <= n; j += lowbit(j)) {
            tree[i][j] += v;
        }
    }
}

int sum(int x, int y) {
    int ret = 0;
    for (int i = x; i > 0; i -= lowbit(i)) {
        for (int j = y; j > 0; j -= lowbit(j)) {
            ret += tree[i][j];
        }
    }
    return ret;
}

int query(int a, int b, int c, int d) {
    return sum(c, d) - sum(a - 1, d) - sum(c, b - 1) + sum(a - 1, b - 1);
}

void compute(int ql, int qr, int vl, int vr) {
    if (ql > qr) {
        return;
    }
    if (vl == vr) {
        for (int i = ql; i <= qr; i++) {
            ans[qid[i]] = xyv[vl].v;
        }
    } else {
        int mid = (vl + vr) >> 1;
        for (int i = vl; i <= mid; i++) {
            add(xyv[i].x, xyv[i].y, 1);
        }
        int lsiz = 0, rsiz = 0;
        for (int i = ql; i <= qr; i++) {
            int id = qid[i];
            int satisfy = query(a[id], b[id], c[id], d[id]);
            if (satisfy >= k[id]) {
                lset[++lsiz] = id;
            } else {
                k[id] -= satisfy;
                rset[++rsiz] = id;
            }
        }
        for (int i = 1; i <= lsiz; i++) {
            qid[ql + i - 1] = lset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            qid[ql + lsiz + i - 1] = rset[i];
        }
        for (int i = vl; i <= mid; i++) {
            add(xyv[i].x, xyv[i].y, -1);
        }
        compute(ql, ql + lsiz - 1, vl, mid);
        compute(ql + lsiz, qr, mid + 1, vr);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            xyv[++cntv].x = i;
            xyv[cntv].y = j;
            cin >> xyv[cntv].v;
        }
    }
    for (int i = 1; i <= q; i++) {
        qid[i] = i;
        cin >> a[i] >> b[i] >> c[i] >> d[i] >> k[i];
    }
    sort(xyv + 1, xyv + cntv + 1, NumberCmp);
    compute(1, q, 1, cntv);
    for (int i = 1; i <= q; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
