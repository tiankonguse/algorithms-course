
#include <algorithm>
#include <cstdio>
#include <cstdlib>
using namespace std;

const int MAXN = 500005;
const int INF = 1000000001;

int head = 0;
int arr[MAXN];
int num[MAXN];
int fa[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
int space[MAXN], si;
int sum[MAXN];
int all[MAXN];
int pre[MAXN];
int suf[MAXN];
bool update[MAXN];
int change[MAXN];
bool rev[MAXN];

void up(int i) {
    int l = ls[i], r = rs[i];
    siz[i] = siz[l] + siz[r] + 1;
    sum[i] = sum[l] + sum[r] + num[i];
    all[i] = max(max(all[l], all[r]), suf[l] + num[i] + pre[r]);
    pre[i] = max(pre[l], sum[l] + num[i] + pre[r]);
    suf[i] = max(suf[r], suf[l] + num[i] + sum[r]);
}

int lr(int i) {
    return rs[fa[i]] == i ? 1 : 0;
}

void rotate(int i) {
    int f = fa[i], g = fa[f], soni = lr(i), sonf = lr(f);
    if (soni == 1) {
        rs[f] = ls[i];
        if (rs[f] != 0) {
            fa[rs[f]] = f;
        }
        ls[i] = f;
    } else {
        ls[f] = rs[i];
        if (ls[f] != 0) {
            fa[ls[f]] = f;
        }
        rs[i] = f;
    }
    if (g != 0) {
        if (sonf == 1) {
            rs[g] = i;
        } else {
            ls[g] = i;
        }
    }
    fa[f] = i;
    fa[i] = g;
    up(f);
    up(i);
}

void splay(int i, int goal) {
    int __it = 0;
    while (fa[i] != goal) {
        if (++__it > 100000) { fprintf(stderr, "SPLAY HANG it=%d i=%d goal=%d head=%d fa=%d\n", __it, i, goal, head, fa[i]); fflush(stderr); exit(1); }
        int f = fa[i], g = fa[f];
        if (g != goal) {
            if (lr(i) == lr(f)) {
                rotate(f);
            } else {
                rotate(i);
            }
        }
        rotate(i);
    }
    if (goal == 0) {
        head = i;
    }
}

void setValue(int i, int val) {
    if (i != 0) {
        update[i] = true;
        change[i] = val;
        num[i] = val;
        sum[i] = siz[i] * val;
        all[i] = max(sum[i], val);
        pre[i] = max(sum[i], 0);
        suf[i] = max(sum[i], 0);
    }
}

void setReverse(int i) {
    if (i != 0) {
        swap(pre[i], suf[i]);
        rev[i] ^= 1;
    }
}

void down(int i) {
    if (update[i]) {
        setValue(ls[i], change[i]);
        setValue(rs[i], change[i]);
        update[i] = false;
    }
    if (rev[i]) {
        swap(ls[i], rs[i]);
        setReverse(ls[i]);
        setReverse(rs[i]);
        rev[i] = false;
    }
}

int init(int val) {
    int i = space[si--];
    siz[i] = 1;
    num[i] = sum[i] = all[i] = val;
    pre[i] = suf[i] = max(val, 0);
    fa[i] = ls[i] = rs[i] = 0;
    update[i] = rev[i] = false;
    return i;
}

int build(int l, int r) {
    int mid = (l + r) / 2;
    int root = init(arr[mid]);
    if (l < mid) {
        ls[root] = build(l, mid - 1);
        fa[ls[root]] = root;
    }
    if (mid < r) {
        rs[root] = build(mid + 1, r);
        fa[rs[root]] = root;
    }
    up(root);
    return root;
}

int find(int rank) {
    int i = head;
    while (i != 0) {
        down(i);
        if (siz[ls[i]] + 1 == rank) {
            return i;
        } else if (siz[ls[i]] >= rank) {
            i = ls[i];
        } else {
            rank -= siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    return 0;
}

void insert(int rank, int n) {
    if (rank == 0) {
        head = build(1, n);
    } else {
        int l = find(rank);
        int r = find(rank + 1);
        splay(l, 0);
        splay(r, l);
        ls[r] = build(1, n);
        fa[ls[r]] = r;
        up(r);
        up(l);
    }
}

void recycle(int i) {
    if (i != 0) {
        space[++si] = i;
        recycle(ls[i]);
        recycle(rs[i]);
    }
}

void remove(int rank, int n) {
    int l = find(rank - 1);
    int r = find(rank + n);
    splay(l, 0);
    splay(r, l);
    recycle(ls[r]);
    ls[r] = 0;
    up(r);
    up(l);
}

void reset(int rank, int n, int val) {
    int l = find(rank - 1);
    int r = find(rank + n);
    splay(l, 0);
    splay(r, l);
    setValue(ls[r], val);
    up(r);
    up(l);
}

void reverse(int rank, int n) {
    int l = find(rank - 1);
    int r = find(rank + n);
    splay(l, 0);
    splay(r, l);
    setReverse(ls[r]);
    up(r);
    up(l);
}

int querySum(int rank, int n) {
    int l = find(rank - 1);
    int r = find(rank + n);
    splay(l, 0);
    splay(r, l);
    return sum[ls[r]];
}

int queryMax() {
    return all[head];
}

