#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstring>

using namespace std;

const double ALPHA = 0.7;
const int MAXN = 100001;
int head = 0;
int cnt = 0;
int key[MAXN];
int key_count[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
int diff[MAXN];
int collect[MAXN];
int ci;
int top;
int father;
int side;

int init(int num) {
    key[++cnt] = num;
    ls[cnt] = rs[cnt] = 0;
    key_count[cnt] = siz[cnt] = diff[cnt] = 1;
    return cnt;
}

void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
    diff[i] = diff[ls[i]] + diff[rs[i]] + (key_count[i] > 0 ? 1 : 0);
}

void inorder(int i) {
    if (i != 0) {
        inorder(ls[i]);
        if (key_count[i] > 0) {
            collect[++ci] = i;
        }
        inorder(rs[i]);
    }
}

int build(int l, int r) {
    if (l > r) {
        return 0;
    }
    int m = (l + r) / 2;
    int h = collect[m];
    ls[h] = build(l, m - 1);
    rs[h] = build(m + 1, r);
    up(h);
    return h;
}

void rebuild() {
    if (top != 0) {
        ci = 0;
        inorder(top);
        if (ci > 0) {
            if (father == 0) {
                head = build(1, ci);
            } else if (side == 1) {
                ls[father] = build(1, ci);
            } else {
                rs[father] = build(1, ci);
            }
        }
    }
}

bool balance(int i) {
    return ALPHA * diff[i] >= max(diff[ls[i]], diff[rs[i]]);
}

void add(int i, int f, int s, int num) {
    if (i == 0) {
        if (f == 0) {
            head = init(num);
        } else if (s == 1) {
            ls[f] = init(num);
        } else {
            rs[f] = init(num);
        }
    } else {
        if (key[i] == num) {
            key_count[i]++;
        } else if (key[i] > num) {
            add(ls[i], i, 1, num);
        } else {
            add(rs[i], i, 2, num);
        }
        up(i);
        if (!balance(i)) {
            top = i;
            father = f;
            side = s;
        }
    }
}

void add(int num) {
    top = father = side = 0;
    add(head, 0, 0, num);
    rebuild();
}

int small(int i, int num) {
    if (i == 0) {
        return 0;
    }
    if (key[i] >= num) {
        return small(ls[i], num);
    } else {
        return siz[ls[i]] + key_count[i] + small(rs[i], num);
    }
}

int getRank(int num) {
    return small(head, num) + 1;
}

int index(int i, int x) {
    if (siz[ls[i]] >= x) {
        return index(ls[i], x);
    } else if (siz[ls[i]] + key_count[i] < x) {
        return index(rs[i], x - siz[ls[i]] - key_count[i]);
    }
    return key[i];
}

int index(int x) {
    return index(head, x);
}

int pre(int num) {
    int kth = getRank(num);
    if (kth == 1) {
        return INT_MIN;
    } else {
        return index(kth - 1);
    }
}

int post(int num) {
    int kth = getRank(num + 1);
    if (kth == siz[head] + 1) {
        return INT_MAX;
    } else {
        return index(kth);
    }
}

void remove(int i, int f, int s, int num) {
    if (key[i] == num) {
        key_count[i]--;
    } else if (key[i] > num) {
        remove(ls[i], i, 1, num);
    } else {
        remove(rs[i], i, 2, num);
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
}

void remove(int num) {
    if (getRank(num) != getRank(num + 1)) {
        top = father = side = 0;
        remove(head, 0, 0, num);
        rebuild();
    }
}

void clear() {
    memset(key, 0, sizeof(key));
    memset(key_count, 0, sizeof(key_count));
    memset(ls, 0, sizeof(ls));
    memset(rs, 0, sizeof(rs));
    memset(siz, 0, sizeof(siz));
    memset(diff, 0, sizeof(diff));
    cnt = 0;
    head = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            add(x);
        } else if (op == 2) {
            remove(x);
        } else if (op == 3) {
            cout << getRank(x) << "\n";
        } else if (op == 4) {
            cout << index(x) << "\n";
        } else if (op == 5) {
            cout << pre(x) << "\n";
        } else {
            cout << post(x) << "\n";
        }
    }
    clear();
    return 0;
}
