#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <climits>
using namespace std;

const int MAXN = 100001;
int head = 0;
int cnt = 0;
int key[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
double priority[MAXN];

void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
}

void split(int l, int r, int i, int num) {
    if (i == 0) {
        rs[l] = ls[r] = 0;
    } else {
        if (key[i] <= num) {
            rs[l] = i;
            split(i, r, rs[i], num);
        } else {
            ls[r] = i;
            split(l, i, ls[i], num);
        }
        up(i);
    }
}

int merge(int l, int r) {
    if (l == 0 || r == 0) {
        return l + r;
    }
    if (priority[l] >= priority[r]) {
        rs[l] = merge(rs[l], r);
        up(l);
        return l;
    } else {
        ls[r] = merge(l, ls[r]);
        up(r);
        return r;
    }
}

void add(int num) {
    split(0, 0, head, num);
    key[++cnt] = num;
    siz[cnt] = 1;
    priority[cnt] = (double)rand() / RAND_MAX;
    head = merge(merge(rs[0], cnt), ls[0]);
}

void remove(int num) {
    split(0, 0, head, num);
    int lm = rs[0];
    int r = ls[0];
    split(0, 0, lm, num - 1);
    int l = rs[0];
    int m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m])), r);
}

int getRank(int num) {
    split(0, 0, head, num - 1);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0]);
    return ans;
}

int index(int i, int x) {
    if (siz[ls[i]] >= x) {
        return index(ls[i], x);
    } else if (siz[ls[i]] + 1 < x) {
        return index(rs[i], x - siz[ls[i]] - 1);
    } else {
        return key[i];
    }
}

int index(int x) {
    return index(head, x);
}

int pre(int i, int num) {
    if (i == 0) {
        return INT_MIN;
    }
    if (key[i] >= num) {
        return pre(ls[i], num);
    } else {
        return max(key[i], pre(rs[i], num));
    }
}

int pre(int num) {
    return pre(head, num);
}

int post(int i, int num) {
    if (i == 0) {
        return INT_MAX;
    }
    if (key[i] <= num) {
        return post(rs[i], num);
    } else {
        return min(key[i], post(ls[i], num));
    }
}

int post(int num) {
    return post(head, num);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(0));
    int n;
    cin >> n;
    for (int i = 1, op, x; i <= n; i++) {
        cin >> op >> x;
        if (op == 1) {
            add(x);
        } else if (op == 2) {
            remove(x);
        } else if (op == 3) {
            cout << getRank(x) << endl;
        } else if (op == 4) {
            cout << index(x) << endl;
        } else if (op == 5) {
            cout << pre(x) << endl;
        } else {
            cout << post(x) << endl;
        }
    }
    return 0;
}
