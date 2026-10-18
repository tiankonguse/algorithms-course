// 狡猾的商人，带权并查集模版题2
// n 个月的收入，m 个操作 l r v 表示第 l 到 r 月总收入为 v
// 判断给定数据是否自洽，矛盾打印 false
// 1 <= n <= 100，1 <= m <= 1000，总收入不超过 int 范围
// 测试链接 : https://www.luogu.com.cn/problem/P2294
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 102;

int t, n, m;
bool ans;
int father[MAXN];
int dist[MAXN];

void prepare() {
    ans = true;
    for (int i = 1; i <= n; i++) {
        father[i] = i;
        dist[i] = 0;
    }
}

int find(int i) {
    if (i != father[i]) {
        int tmp = father[i];
        father[i] = find(tmp);
        dist[i] += dist[tmp];
    }
    return father[i];
}

void unionSet(int l, int r, int v) {
    int lf = find(l), rf = find(r);
    if (lf != rf) {
        father[lf] = rf;
        dist[lf] = v + dist[r] - dist[l];
    }
}

bool check(int l, int r, int v) {
    if (find(l) == find(r)) {
        if ((dist[l] - dist[r]) != v) {
            return false;
        }
    }
    return true;
}

int main() {
    scanf("%d", &t);
    for (int c = 1; c <= t; c++) {
        scanf("%d%d", &n, &m);
        n++;
        prepare();
        for (int i = 1, l, r, v; i <= m; i++) {
            scanf("%d%d%d", &l, &r, &v);
            r++;
            if (!check(l, r, v)) {
                ans = false;
            } else {
                unionSet(l, r, v);
            }
        }
        printf("%s\n", ans ? "true" : "false");
    }
    return 0;
}
