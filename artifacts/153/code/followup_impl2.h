
#include <climits>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
using namespace std;

const int MAXN = 2000001;

int head = 0;
int cnt = 0;
int key[MAXN];
int fa[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];

void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + 1;
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
    int f = fa[i], g = fa[f];
    while (f != goal) {
        if (g != goal) {
            if (lr(i) == lr(f)) {
                rotate(f);
            } else {
                rotate(i);
            }
        }
        rotate(i);
        f = fa[i];
        g = fa[f];
    }
    if (goal == 0) {
        head = i;
    }
}

int find(int rank) {
    int i = head;
    while (i != 0) {
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

void add(int num) {
    key[++cnt] = num;
    siz[cnt] = 1;
    if (head == 0) {
        head = cnt;
    } else {
        int f = 0, i = head, son = 0;
        while (i != 0) {
            f = i;
            if (key[i] <= num) {
                son = 1;
                i = rs[i];
            } else {
                son = 0;
                i = ls[i];
            }
        }
        if (son == 1) {
            rs[f] = cnt;
        } else {
            ls[f] = cnt;
        }
        fa[cnt] = f;
        splay(cnt, 0);
    }
}

int getRank(int num) {
    int i = head, last = head;
    int ans = 0;
    while (i != 0) {
        last = i;
        if (key[i] >= num) {
            i = ls[i];
        } else {
            ans += siz[ls[i]] + 1;
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans + 1;
}

int index(int x) {
    int i = find(x);
    splay(i, 0);
    return key[i];
}

int pre(int num) {
    int i = head, last = head;
    int ans = INT_MIN;
    while (i != 0) {
        last = i;
        if (key[i] >= num) {
            i = ls[i];
        } else {
            ans = max(ans, key[i]);
            i = rs[i];
        }
    }
    splay(last, 0);
    return ans;
}

int post(int num) {
    int i = head, last = head;
    int ans = INT_MAX;
    while (i != 0) {
        last = i;
        if (key[i] <= num) {
            i = rs[i];
        } else {
            ans = min(ans, key[i]);
            i = ls[i];
        }
    }
    splay(last, 0);
    return ans;
}

void remove(int num) {
    int kth = getRank(num);
    if (kth != getRank(num + 1)) {
        int i = find(kth);
        splay(i, 0);
        if (ls[i] == 0) {
            head = rs[i];
        } else if (rs[i] == 0) {
            head = ls[i];
        } else {
            int j = find(kth + 1);
            splay(j, i);
            ls[j] = ls[i];
            fa[ls[j]] = j;
            up(j);
            head = j;
        }
        if (head != 0) {
            fa[head] = 0;
        }
    }
}

