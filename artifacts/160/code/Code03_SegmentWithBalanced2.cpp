#include <bits/stdc++.h>

using namespace std;

const int MAXN = 200001;
const int MAXT = MAXN * 40;
const int INF = INT_MAX;
int n, m;
int arr[MAXN];

int cntn;
int root[MAXN << 2];
int key[MAXT];
int ls[MAXT];
int rs[MAXT];
bool alive[MAXT];
int aliveSiz[MAXT];

double ALPHA = 0.7;
int top;
int father;
int side;
int collect[MAXN];
int collectSiz;

int init(int num) {
    key[++cntn] = num;
    ls[cntn] = rs[cntn] = 0;
    alive[cntn] = true;
    aliveSiz[cntn] = 1;
    return cntn;
}

void up(int i) {
    aliveSiz[i] = (alive[i] ? 1 : 0) + aliveSiz[ls[i]] + aliveSiz[rs[i]];
}

bool balance(int i) {
    return ALPHA * aliveSiz[i] >= max(aliveSiz[ls[i]], aliveSiz[rs[i]]);
}

void inorder(int i) {
    if (i != 0 && aliveSiz[i] != 0) {
        inorder(ls[i]);
        if (alive[i]) {
            collect[++collectSiz] = i;
        }
        inorder(rs[i]);
    }
}

int build(int l, int r) {
    if (l > r) {
        return 0;
    }
    int mid = (l + r) / 2;
    int h = collect[mid];
    ls[h] = build(l, mid - 1);
    rs[h] = build(mid + 1, r);
    up(h);
    return h;
}

int treeAdd(int i, int f, int s, int num) {
    if (i == 0 || aliveSiz[i] == 0) {
        return init(num);
    }
    if (num <= key[i]) {
        ls[i] = treeAdd(ls[i], i, 1, num);
    } else {
        rs[i] = treeAdd(rs[i], i, 2, num);
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
    return i;
}

int treeSmall(int i, int num) {
    if (i == 0 || aliveSiz[i] == 0) {
        return 0;
    }
    if (num <= key[i]) {
        return treeSmall(ls[i], num);
    } else {
        return aliveSiz[ls[i]] + (alive[i] ? 1 : 0) + treeSmall(rs[i], num);
    }
}

int treeIndex(int i, int x) {
    if (x <= aliveSiz[ls[i]]) {
        return treeIndex(ls[i], x);
    } else {
        int less = aliveSiz[ls[i]] + (alive[i] ? 1 : 0);
        if (less < x) {
            return treeIndex(rs[i], x - less);
        }
    }
    return key[i];
}

void treeRemove(int i, int f, int s, int rank) {
    int lsiz = aliveSiz[ls[i]];
    if (rank <= lsiz) {
        treeRemove(ls[i], i, 1, rank);
    } else {
        int cur = alive[i] ? 1 : 0;
        if (alive[i] && rank == lsiz + cur) {
            alive[i] = false;
        } else {
            treeRemove(rs[i], i, 2, rank - lsiz - cur);
        }
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
}

void rebuild(int version) {
    if (top != 0) {
        collectSiz = 0;
        inorder(top);
        int newRoot = build(1, collectSiz);
        if (father == 0) {
            root[version] = newRoot;
        } else if (side == 1) {
            ls[father] = newRoot;
        } else {
            rs[father] = newRoot;
        }
    }
}

void innerAdd(int version, int num) {
    top = father = side = 0;
    root[version] = treeAdd(root[version], 0, 0, num);
    rebuild(version);
}

int innerRank(int version, int num) {
    return treeSmall(root[version], num) + 1;
}

void innerRemove(int version, int num) {
    int rank1 = innerRank(version, num);
    int rank2 = innerRank(version, num + 1);
    if (rank1 != rank2) {
        top = father = side = 0;
        treeRemove(root[version], 0, 0, rank1);
        rebuild(version);
    }
}

int innerPre(int version, int num) {
    int rank = innerRank(version, num);
    if (rank == 1) {
        return -INF;
    } else {
        return treeIndex(root[version], rank - 1);
    }
}

int innerPost(int version, int num) {
    int rank = innerRank(version, num + 1);
    if (rank == aliveSiz[root[version]] + 1) {
        return INF;
    } else {
        return treeIndex(root[version], rank);
    }
}

void add(int jobi, int jobv, int l, int r, int i) {
    innerAdd(i, jobv);
    if (l < r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            add(jobi, jobv, l, mid, i << 1);
        } else {
            add(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
    }
}

void update(int jobi, int jobv, int l, int r, int i) {
    innerRemove(i, arr[jobi]);
    innerAdd(i, jobv);
    if (l < r) {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            update(jobi, jobv, l, mid, i << 1);
        } else {
            update(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
    }
}

int small(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return treeSmall(root[i], jobv);
    }
    int mid = (l + r) >> 1;
    int ans = 0;
    if (jobl <= mid) {
        ans += small(jobl, jobr, jobv, l, mid, i << 1);
    }
    if (jobr > mid) {
        ans += small(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
    }
    return ans;
}

int number(int jobl, int jobr, int jobk) {
    int l = 0, r = 100000000, mid, ans = 0;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (small(jobl, jobr, mid + 1, 1, n, 1) + 1 > jobk) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

int pre(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return innerPre(i, jobv);
    }
    int mid = (l + r) >> 1;
    int ans = -INF;
    if (jobl <= mid) {
        ans = max(ans, pre(jobl, jobr, jobv, l, mid, i << 1));
    }
    if (jobr > mid) {
        ans = max(ans, pre(jobl, jobr, jobv, mid + 1, r, i << 1 | 1));
    }
    return ans;
}

int post(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return innerPost(i, jobv);
    }
    int mid = (l + r) >> 1;
    int ans = INF;
    if (jobl <= mid) {
        ans = min(ans, post(jobl, jobr, jobv, l, mid, i << 1));
    }
    if (jobr > mid) {
        ans = min(ans, post(jobl, jobr, jobv, mid + 1, r, i << 1 | 1));
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i <= n; i++) {
        add(i, arr[i], 1, n, 1);
    }
    for (int i = 1, op, x, y, z; i <= m; i++) {
        cin >> op >> x >> y;
        if (op == 3) {
            update(x, y, 1, n, 1);
            arr[x] = y;
        } else {
            cin >> z;
            if (op == 1) {
                cout << small(x, y, z, 1, n, 1) + 1 << '\n';
            } else if (op == 2) {
                cout << number(x, y, z) << '\n';
            } else if (op == 4) {
                cout << pre(x, y, z, 1, n, 1) << '\n';
            } else {
                cout << post(x, y, z, 1, n, 1) << '\n';
            }
        }
    }
    return 0;
}
