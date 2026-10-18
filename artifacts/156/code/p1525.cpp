// 关押罪犯
// n 个犯人分配到两个监狱，m 条仇恨记录 l r v
// 每个监狱的暴力值 = 该监狱内最深的仇恨，冲突值 = 两监狱暴力值最大值
// 最小化冲突值，返回最小值
// 贪心：按仇恨值降序，两人必须分到不同监狱（enemy 数组维护），
// 出现 must 同监狱的两人时，当前仇恨值就是答案
// 1 <= n <= 2e4，1 <= m <= 1e5，1 <= 仇恨值 <= 1e9
// 测试链接 : https://www.luogu.com.cn/problem/P1525
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 20002;
const int MAXM = 100001;

int n, m;
int father[MAXN];
int enemyArr[MAXN];
struct Edge { int l, r, v; };
Edge arr[MAXM];

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

bool same(int l, int r) {
    return find(l) == find(r);
}

int compute() {
    sort(arr + 1, arr + m + 1, [](const Edge& a, const Edge& b) {
        return a.v > b.v;
    });
    int ans = 0;
    for (int i = 1; i <= m; i++) {
        int l = arr[i].l, r = arr[i].r, v = arr[i].v;
        if (same(l, r)) {
            ans = v;
            break;
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
    return ans;
}

int main() {
    scanf("%d%d", &n, &m);
    prepare();
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &arr[i].l, &arr[i].r, &arr[i].v);
    }
    printf("%d\n", compute());
    return 0;
}
