// 团伙
// n 个黑帮成员，m 条事实：F l r 朋友、E l r 敌人
// 敌人的敌人是朋友，朋友同黑帮，敌人不同黑帮，计算黑帮个数
// 经典并查集分类处理：E 时互挂 enemy 数组，无法推断的成员自己一个黑帮
// 1 <= n <= 1000，1 <= m <= 5000
// 测试链接 : https://www.luogu.com.cn/problem/P1892
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1001;

int n, m;
int father[MAXN];
int enemyArr[MAXN];

void prepare() {
    for (int i = 1; i <= n; i++) {
        father[i] = i;
        enemyArr[i] = 0;
    }
}

int find(int i) {
    father[i] = father[i] == i ? i : find(father[i]);
    return father[i];
}

void unionSet(int l, int r) {
    father[find(l)] = find(r);
}

int main() {
    scanf("%d%d", &n, &m);
    prepare();
    char op;
    int l, r;
    for (int i = 1; i <= m; i++) {
        scanf(" %c%d%d", &op, &l, &r);
        if (op == 'F') {
            unionSet(l, r);
        } else {
            if (enemyArr[l] == 0) {
                enemyArr[l] = r;
            } else {
                unionSet(enemyArr[l], r);
            }
            if (enemyArr[r] == 0) {
                enemyArr[r] = l;
            } else {
                unionSet(l, enemyArr[r]);
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (i == father[i]) {
            ans++;
        }
    }
    printf("%d\n", ans);
    return 0;
}
