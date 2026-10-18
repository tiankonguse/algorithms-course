// 错误答案数量，带权并查集模版题3
// n 个数字，m 个操作 l r v 表示 l~r 累加和为 v
// 与之前信息矛盾的操作为错误操作，不执行，打印错误操作数量
// 1 <= n <= 2e5，1 <= m <= 4e4，多测到 EOF
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=3038
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200002;

int n, m, ans;
int father[MAXN];
int dist[MAXN];

void prepare() {
    ans = 0;
    for (int i = 0; i <= n; i++) {
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
    while (scanf("%d%d", &n, &m) == 2) {
        n++;
        prepare();
        for (int i = 1, l, r, v; i <= m; i++) {
            scanf("%d%d%d", &l, &r, &v);
            r++;
            if (!check(l, r, v)) {
                ans++;
            } else {
                unionSet(l, r, v);
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
