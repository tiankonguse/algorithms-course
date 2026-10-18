#include <bits/stdc++.h>

using namespace std;

const int MAXN = 2000001;

int cntn;
int root;
int key[MAXN];
int ls[MAXN];
int rs[MAXN];

 // 节点是否存活，删掉就算死亡
bool alive[MAXN];

 // 子树的存活节点数量
int aliveSiz[MAXN];

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

 // 汇总存活节点数量
void up(int i) {
    aliveSiz[i] = (alive[i] ? 1 : 0) + aliveSiz[ls[i]] + aliveSiz[rs[i]];
}

void inorder(int i) {
    // 增加剪枝：整棵树上没有存活节点也跳过
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
    int m = (l + r) / 2;
    int h = collect[m];
    ls[h] = build(l, m - 1);
    rs[h] = build(m + 1, r);
    up(h);
    return h;
}

void rebuild() {
    if (top != 0) {
        collectSiz = 0;
        inorder(top);
        int newRoot = build(1, collectSiz);
        if (father == 0) {
            root = newRoot;
        } else if (side == 1) {
            ls[father] = newRoot;
        } else {
            rs[father] = newRoot;
        }
    }
}

bool balance(int i) {
    return ALPHA * aliveSiz[i] >= max(aliveSiz[ls[i]], aliveSiz[rs[i]]);
}

 // 返回头节点编号
int add(int i, int f, int s, int num) {
    // 增加剪枝：整棵树上没有存活节点就算空树
    if (i == 0 || aliveSiz[i] == 0) {
        return init(num);
    }
    if (num <= key[i]) {
        ls[i] = add(ls[i], i, 1, num);
    } else {
        rs[i] = add(rs[i], i, 2, num);
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
    return i;
}

void add(int num) {
    top = father = side = 0;
    root = add(root, 0, 0, num);
    rebuild();
}

int small(int i, int num) {
    // 增加剪枝：整棵树上没有存活节点，就算空树
    if (i == 0 || aliveSiz[i] == 0) {
        return 0;
    }
    if (num <= key[i]) {
        return small(ls[i], num);
    } else {
        return aliveSiz[ls[i]] + (alive[i] ? 1 : 0) + small(rs[i], num);
    }
}

int getRank(int num) {
    return small(root, num) + 1;
}

int index(int i, int x) {
    if (x <= aliveSiz[ls[i]]) {
        return index(ls[i], x);
    } else {
        int less = aliveSiz[ls[i]] + (alive[i] ? 1 : 0);
        if (less < x) {
            return index(rs[i], x - less);
        }
    }
    return key[i];
}

int index(int x) {
    return index(root, x);
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
    if (kth == aliveSiz[root] + 1) {
        return INT_MAX;
    } else {
        return index(kth);
    }
}

 // 注意remove方法
 // 因为替罪羊树会重构，所以值相同的一批节点，重构时假设选出的头为h
 // 那么这批节点，有可能在h的左侧，也有可能在h的右侧
 // 所以删除时，如果h已经被删，还要继续寻找其他key值相同的节点
 // 此时只根据key值的大小关系，方向无法确定是左还是右
 // 所以先求出目标的排名，再按排名删除，这样移动方向是确定的
void remove(int i, int f, int s, int rank) {
    int lsiz = aliveSiz[ls[i]];
    if (rank <= lsiz) {
        remove(ls[i], i, 1, rank);
    } else {
        int cur = alive[i] ? 1 : 0;
        if (alive[i] && rank == lsiz + cur) {
            alive[i] = false;
        } else {
            remove(rs[i], i, 2, rank - lsiz - cur);
        }
    }
    up(i);
    if (!balance(i)) {
        top = i;
        father = f;
        side = s;
    }
}

void remove(int num) {
    int rank1 = getRank(num);
    int rank2 = getRank(num + 1);
    if (rank1 != rank2) {
        top = father = side = 0;
        remove(root, 0, 0, rank1);
        rebuild();
    }
}
#include <set>
#include <cstdlib>

using namespace std;

// 暴力：multiset
multiset<int> ms;

// 对拍 main：400 轮 × 300 步，x ∈ [-50, 50]
int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 400; round++) {
        ms.clear();
        // 干净重建替罪羊树
        memset(alive, 0, sizeof(alive));
        memset(aliveSiz, 0, sizeof(aliveSiz));
        cntn = 0;
        root = 0;
        for (int step = 0; step < 300; step++) {
            int op = rand() % 6 + 1;
            int x = rand() % 101 - 50;
            if (op == 1) {
                add(x);
                ms.insert(x);
            } else if (op == 2) {
                remove(x);
                auto it = ms.find(x);
                if (it != ms.end()) ms.erase(it);
            } else if (op == 3) {
                total++;
                int got = getRank(x);
                int want = 1 + (int)ms.count(x) + 0; // 占位，下面算
                want = 1 + (int)distance(ms.begin(), ms.lower_bound(x));
                if (got != want) { fails++; printf("MISMATCH op3 round=%d step=%d x=%d got=%d want=%d\n", round, step, x, got, want); }
            } else if (op == 4) {
                if (ms.empty()) continue;
                total++;
                int kth = rand() % (int)ms.size() + 1;
                int got = index(kth);
                auto it = ms.begin();
                advance(it, kth - 1);
                int want = *it;
                if (got != want) { fails++; printf("MISMATCH op4 round=%d step=%d k=%d got=%d want=%d\n", round, step, kth, got, want); }
            } else if (op == 5) {
                total++;
                int got = pre(x);
                int want = INT_MIN;
                auto it = ms.lower_bound(x);
                if (it != ms.begin()) { --it; want = *it; }
                if (got != want) { fails++; printf("MISMATCH op5 round=%d step=%d x=%d got=%d want=%d\n", round, step, x, got, want); }
            } else {
                total++;
                int got = post(x);
                int want = INT_MAX;
                auto it = ms.upper_bound(x);
                if (it != ms.end()) want = *it;
                if (got != want) { fails++; printf("MISMATCH op6 round=%d step=%d x=%d got=%d want=%d\n", round, step, x, got, want); }
            }
        }
    }
    printf("== total %d cases, fails %d ==\n", total, fails);
    return 0;
}
