#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXT = MAXN * 10;
const int MAXV = 100;
int n, q;
int arr[MAXN];

int root[MAXV + 1];
int ls[MAXT];
int rs[MAXT];
bool status[MAXT];

int pool[MAXT];
int top;

void prepare() {
    top = 0;
    for (int i = 1; i < MAXT; i++) {
        pool[++top] = i;
    }
}

int newNode() {
    return pool[top--];
}

void del(int i) {
    pool[++top] = i;
    ls[i] = 0;
    rs[i] = 0;
    status[i] = false;
}

void up(int i) {
    status[i] = status[ls[i]] | status[rs[i]];
}

int insert(int jobi, int l, int r, int i) {
    int rt = i;
    if (rt == 0) {
        rt = newNode();
    }
    if (l == r) {
        status[rt] = true;
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            ls[rt] = insert(jobi, l, mid, ls[rt]);
        } else {
            rs[rt] = insert(jobi, mid + 1, r, rs[rt]);
        }
        up(rt);
    }
    return rt;
}

int merge(int l, int r, int t1, int t2) {
    if (t1 == 0 || t2 == 0) {
        return t1 + t2;
    }
    if (l == r) {
        status[t1] |= status[t2];
    } else {
        int mid = (l + r) >> 1;
        ls[t1] = merge(l, mid, ls[t1], ls[t2]);
        rs[t1] = merge(mid + 1, r, rs[t1], rs[t2]);
        up(t1);
    }
    del(t2);
    return t1;
}

int tree1, tree2;

void split(int jobl, int jobr, int l, int r, int t1) {
    if (t1 == 0) {
        tree1 = t1;
        tree2 = 0;
        return;
    }
    if (jobl <= l && r <= jobr) {
        tree1 = 0;
        tree2 = t1;
        return;
    }
    int t2 = newNode();
    int mid = (l + r) >> 1;
    if (jobl <= mid) {
        split(jobl, jobr, l, mid, ls[t1]);
        ls[t1] = tree1;
        ls[t2] = tree2;
    }
    if (jobr > mid) {
        split(jobl, jobr, mid + 1, r, rs[t1]);
        rs[t1] = tree1;
        rs[t2] = tree2;
    }
    up(t1);
    up(t2);
    tree1 = t1;
    tree2 = t2;
}

void dfs(int val, int l, int r, int i) {
    if (i == 0 || !status[i]) {
        return;
    }
    if (l == r) {
        arr[l] = val;
    } else {
        int mid = (l + r) >> 1;
        dfs(val, l, mid, ls[i]);
        dfs(val, mid + 1, r, rs[i]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    prepare();
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i <= n; i++) {
        root[arr[i]] = insert(i, 1, n, root[arr[i]]);
    }
    cin >> q;
    for (int i = 1, l, r, x, y; i <= q; i++) {
        cin >> l >> r >> x >> y;
        split(l, r, 1, n, root[x]);
        root[x] = tree1;
        root[y] = merge(1, n, root[y], tree2);
    }
    for (int v = 1; v <= MAXV; v++) {
        dfs(v, 1, n, root[v]);
    }
    for (int i = 1; i <= n; i++) {
        cout << arr[i] << ' ';
    }
    return 0;
}
