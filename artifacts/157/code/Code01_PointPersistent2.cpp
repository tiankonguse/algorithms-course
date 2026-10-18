#include <iostream>

using namespace std;

const int MAXN = 1000001;
const int MAXT = MAXN * 23;
int n, m;
int arr[MAXN];
int root[MAXN];
int ls[MAXT];
int rs[MAXT];
int value[MAXT];
int cnt = 0;

int build(int l, int r) {
    int rt = ++cnt;
    if (l == r) {
        value[rt] = arr[l];
    } else {
        int mid = (l + r) >> 1;
        ls[rt] = build(l, mid);
        rs[rt] = build(mid + 1, r);
    }
    return rt;
}

int update(int jobi, int jobv, int l, int r, int i) {
    int rt = ++cnt;
    ls[rt] = ls[i];
    rs[rt] = rs[i];
    value[rt] = value[i];
    if (l == r) {
        value[rt] = jobv;
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = update(jobi, jobv, l, mid, ls[rt]);
        } else {
            rs[rt] = update(jobi, jobv, mid + 1, r, rs[rt]);
        }
    }
    return rt;
}

int query(int jobi, int l, int r, int i) {
    if (l == r) {
        return value[i];
    }
    int mid = (l + r) >> 1;
    if (jobi <= mid) {
        return query(jobi, l, mid, ls[i]);
    } else {
        return query(jobi, mid + 1, r, rs[i]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    root[0] = build(1, n);
    for (int i = 1, version, op, x, y; i <= m; i++) {
        cin >> version >> op >> x;
        if (op == 1) {
            cin >> y;
            root[i] = update(x, y, 1, n, root[version]);
        } else {
            root[i] = root[version];
            cout << query(x, 1, n, root[i]) << '\n';
        }
    }
    return 0;
}
