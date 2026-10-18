
using namespace std;

const int MAXN = 80005;

int head = 0;
int cnt = 0;
int num[MAXN];
int fa[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
int pos[MAXN];

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

void add(int s) {
    num[++cnt] = s;
    pos[s] = cnt;
    siz[cnt] = 1;
    fa[cnt] = head;
    rs[head] = cnt;
    splay(cnt, 0);
}

int ask(int s) {
    int i = pos[s];
    splay(i, 0);
    return siz[ls[i]];
}

int query(int s) {
    int i = find(s);
    splay(i, 0);
    return num[i];
}

void move(int a, int b) {
    int l = find(a - 1);
    int r = find(a + 1);
    splay(l, 0);
    splay(r, l);
    int i = ls[r];
    ls[r] = 0;
    up(r);
    up(l);
    l = find(b - 1);
    r = find(b);
    splay(l, 0);
    splay(r, l);
    ls[r] = i;
    fa[i] = r;
    up(r);
    up(l);
}

