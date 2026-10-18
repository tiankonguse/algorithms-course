#include <bits/stdc++.h>

using namespace std;

const int MAXN = 500001;
const int BIT = 21;
int n, q;

int arr[MAXN];
int qid[MAXN];
int l[MAXN];
int r[MAXN];

int baset[MAXN][BIT + 1];
int tmp[BIT + 1];

int lset[MAXN];
int rset[MAXN];

int ans[MAXN];

void insert(int* basis, int num) {
    for (int i = BIT; i >= 0; i--) {
        if ((num >> i) & 1) {
            if (basis[i] == 0) {
                basis[i] = num;
                return;
            }
            num ^= basis[i];
        }
    }
}

void clear(int* basis) {
    for (int i = 0; i <= BIT; i++) {
        basis[i] = 0;
    }
}

int maxEor(int* basis) {
    int ret = 0;
    for (int i = BIT; i >= 0; i--) {
        ret = max(ret, ret ^ basis[i]);
    }
    return ret;
}

void clone(int* b1, int* b2) {
    for (int i = 0; i <= BIT; i++) {
        b1[i] = b2[i];
    }
}

void merge(int* b1, int* b2) {
    clone(tmp, b1);
    for (int i = 0; i <= BIT; i++) {
        insert(tmp, b2[i]);
    }
}

void compute(int ql, int qr, int vl, int vr) {
    if (ql > qr) {
        return;
    }
    if (vl == vr) {
        for (int i = ql; i <= qr; i++) {
            ans[qid[i]] = arr[vl];
        }
    } else {
        int mid = (vl + vr) >> 1;
        clear(baset[mid]);
        insert(baset[mid], arr[mid]);
        for (int i = mid - 1; i >= vl; i--) {
            clone(baset[i], baset[i + 1]);
            insert(baset[i], arr[i]);
        }
        for (int i = mid + 1; i <= vr; i++) {
            clone(baset[i], baset[i - 1]);
            insert(baset[i], arr[i]);
        }
        int lsiz = 0, rsiz = 0;
        for (int i = ql, id; i <= qr; i++) {
            id = qid[i];
            if (r[id] < mid) {
                lset[++lsiz] = id;
            } else if (l[id] > mid) {
                rset[++rsiz] = id;
            } else {
                merge(baset[l[id]], baset[r[id]]);
                ans[id] = maxEor(tmp);
            }
        }
        for (int i = 1; i <= lsiz; i++) {
            qid[ql + i - 1] = lset[i];
        }
        for (int i = 1; i <= rsiz; i++) {
            qid[ql + lsiz + i - 1] = rset[i];
        }
        compute(ql, ql + lsiz - 1, vl, mid);
        compute(ql + lsiz, ql + lsiz + rsiz - 1, mid + 1, vr);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    cin >> q;
    for (int i = 1; i <= q; i++) {
        qid[i] = i;
        cin >> l[i] >> r[i];
    }
    compute(1, q, 1, n);
    for (int i = 1; i <= q; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
