#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 300001;

int head = 0;
int cnt = 0;
int key[MAXN];
int fa[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
int limit;
int change = 0;
int enter = 0;

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

int index(int x) {
    int i = head, last = head;
    while (i != 0) {
        last = i;
        if (siz[ls[i]] >= x) {
            i = ls[i];
        } else if (siz[ls[i]] + 1 < x) {
            x -= siz[ls[i]] + 1;
            i = rs[i];
        } else {
            i = 0;
        }
    }
    splay(last, 0);
    return key[last];
}

void departure() {
    int num = limit - change - 1;
    int i = head, ans = 0;
    while (i != 0) {
        if (key[i] > num) {
            ans = i;
            i = ls[i];
        } else {
            i = rs[i];
        }
    }
    if (ans == 0) {
        head = 0;
    } else {
        splay(ans, 0);
        ls[head] = 0;
        up(head);
    }
}

