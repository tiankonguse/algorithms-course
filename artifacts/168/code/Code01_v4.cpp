#include <bits/stdc++.h>

using namespace std;

struct Number {
    int i, v;
};

bool NumberCmp(Number x, Number y) {
    return x.v < y.v;
}

const int MAXN = 200001;
int n, m;

Number arr[MAXN];

int qid[MAXN];
int l[MAXN];
int r[MAXN];
int k[MAXN];

int tree[MAXN];
int used = 0;

int lset[MAXN];
int rset[MAXN];

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

void compute(int ql, int qr, int vl, int vr) {
    if (ql > qr) {
        return;
    }
    if (vl == vr) {
        for (int i = ql; i <= qr; i++) {
            ans[qid[i]] = arr[vl].v;
        }
    } else {
        int mid = (vl + vr) / 2;
        while (used < mid) {
            used++;
            add(arr[used].i, 1);
        }
        while (used > mid) {
            add(arr[used].i, -1);
            used--;
        }
        int lsiz = 0, rsiz = 0;
        for (int i = ql; i <= qr; i++) {
            int id = qid[i];
            int satisfy = query(l[id], r[id]);
            if (satisfy >= k[id]) {
                lset[++lsiz] = id;
            } else {
                rset[++rsiz] = id;
            }
        }
        for (int i = 1; i <= lsiz; i++) {
            qid[ql + i - 1] = lset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            qid[ql + lsiz + i - 1] = rset[i];
        }
        compute(ql, ql + lsiz - 1, vl, mid);
        compute(ql + lsiz, qr, mid + 1, vr);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        arr[i].i = i;
        cin >> arr[i].v;
    }
    for (int i = 1; i <= m; i++) {
        qid[i] = i;
        cin >> l[i] >> r[i] >> k[i];
    }
    sort(arr + 1, arr + n + 1, NumberCmp);
    compute(1, m, 1, n);
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
