
using namespace std;

const int MAXN = 100005;

int head = 0;
int cnt = 0;
int num[MAXN];
int fa[MAXN];
int ls[MAXN];
int rs[MAXN];
int siz[MAXN];
bool rev[MAXN];
int sta[MAXN];
int si;
int ans[MAXN];
int ai;

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

void down(int i) {
    if (rev[i]) {
        rev[ls[i]] = !rev[ls[i]];
        rev[rs[i]] = !rev[rs[i]];
        int tmp = ls[i];
        ls[i] = rs[i];
        rs[i] = tmp;
        rev[i] = false;
    }
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

void add(int x) {
    num[++cnt] = x;
    siz[cnt] = 1;
    fa[cnt] = head;
    rs[head] = cnt;
    splay(cnt, 0);
}

void reverse(int l, int r) {
    int i = find(l - 1);
    int j = find(r + 1);
    splay(i, 0);
    splay(j, i);
    rev[ls[rs[head]]] = !rev[ls[rs[head]]];
}

void inorder(int i) {
    if (i != 0) {
        down(i);
        inorder(ls[i]);
        ans[++ai] = num[i];
        inorder(rs[i]);
    }
}

void inorder() {
    si = 0;
    int i = head;
    while (si != 0 || i != 0) {
        if (i != 0) {
            down(i);
            sta[++si] = i;
            i = ls[i];
        } else {
            i = sta[si--];
            ans[++ai] = num[i];
            i = rs[i];
        }
    }
}

