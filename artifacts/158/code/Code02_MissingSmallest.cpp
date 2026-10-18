#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXT = MAXN * 22;
int n, m;
int arr[MAXN];
int root[MAXN];
int ls[MAXT];
int rs[MAXT];
int lateLeft[MAXT];
int cnt;

int build(int l, int r) {
    int rt = ++cnt;
    lateLeft[rt] = 0;
    if (l < r) {
        int mid = (l + r) / 2;
        ls[rt] = build(l, mid);
        rs[rt] = build(mid + 1, r);
    }
    return rt;
}

int update(int jobi, int jobv, int l, int r, int i) {
    int rt = ++cnt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    lateLeft[rt] = lateLeft[i];
    if (l == r) {
        lateLeft[rt] = jobv;
    } else {
        int mid = (l + r) / 2;
        if (jobi <= mid) {
            ls[rt] = update(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = update(jobi, jobv, mid + 1, r, rs[rt]);
        }
        lateLeft[rt] = min(lateLeft[ls[rt]], lateLeft[rs[rt]]);
    }
    return rt;
}

int query(int pos, int l, int r, int i) {
    if (l == r) {
        return l;
    }
    int mid = (l + r) / 2;
    if (lateLeft[ls[i]] < pos) {
        return query(pos, l, mid, ls[i]);
    } else {
        return query(pos, mid + 1, r, rs[i]);
    }
}

void prepare() {
    cnt = 0;
    root[0] = build(0, n);
    for (int i = 1; i <= n; i++) {
        if (arr[i] > n || arr[i] < 0) {
            root[i] = root[i - 1];
        } else {
            root[i] = update(arr[i], i, 0, n, root[i - 1]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    prepare();
    for (int i = 1, l, r; i <= m; i++) {
        cin >> l >> r;
        cout << query(l, 0, n, root[r]) << "\n";
    }
    return 0;
}
