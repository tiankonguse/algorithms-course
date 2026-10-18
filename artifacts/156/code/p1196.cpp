// 银河英雄传说
// 30000 艘战舰，初始各自一队，M l r 把 l 队整体移到 r 队末尾，C l r 查询中间隔几艘
// 1 <= t <= 5e5，迭代 find 防爆栈
// 测试链接 : https://www.luogu.com.cn/problem/P1196
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 30001;

int n = 30000;
int father[MAXN];
int dist[MAXN];
int sizeArr[MAXN];
int stackArr[MAXN];

void prepare() {
    for (int i = 1; i <= n; i++) {
        father[i] = i;
        dist[i] = 0;
        sizeArr[i] = 1;
    }
}

int find(int i) {
    int si = 0;
    while (i != father[i]) {
        stackArr[++si] = i;
        i = father[i];
    }
    stackArr[si + 1] = i;
    for (int j = si; j >= 1; j--) {
        father[stackArr[j]] = i;
        dist[stackArr[j]] += dist[stackArr[j + 1]];
    }
    return i;
}

void unionSet(int l, int r) {
    int lf = find(l), rf = find(r);
    if (lf != rf) {
        father[lf] = rf;
        dist[lf] += sizeArr[rf];
        sizeArr[rf] += sizeArr[lf];
    }
}

int query(int l, int r) {
    if (find(l) != find(r)) {
        return -1;
    }
    return abs(dist[l] - dist[r]) - 1;
}

int main() {
    prepare();
    int t;
    scanf("%d", &t);
    char op;
    for (int i = 1, l, r; i <= t; i++) {
        scanf(" %c%d%d", &op, &l, &r);
        if (op == 'M') {
            unionSet(l, r);
        } else {
            printf("%d\n", query(l, r));
        }
    }
    return 0;
}
