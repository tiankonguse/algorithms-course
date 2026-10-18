// 甄别食物链
// n 只动物，A/B/C 三类，A吃B、B吃C、C吃A
// 1 X Y 同类，2 X Y X吃Y，统计假话数量
// 带权并查集维护 %3 余数：0 同类、1 吃头、2 被头吃
// 1 <= n <= 5e4，1 <= k <= 1e5
// 测试链接 : https://www.luogu.com.cn/problem/P2024
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50001;

int n, k, ans;
int father[MAXN];
int dist[MAXN];

void prepare() {
    ans = 0;
    for (int i = 1; i <= n; i++) {
        father[i] = i;
        dist[i] = 0;
    }
}

int find(int i) {
    if (i != father[i]) {
        int tmp = father[i];
        father[i] = find(tmp);
        dist[i] = (dist[i] + dist[tmp]) % 3;
    }
    return father[i];
}

// op == 1：l 和 r 是同类；op == 2：l 吃 r
void unionSet(int op, int l, int r) {
    int lf = find(l), rf = find(r), v = op == 1 ? 0 : 1;
    if (lf != rf) {
        father[lf] = rf;
        dist[lf] = (dist[r] - dist[l] + v + 3) % 3;
    }
}

bool check(int op, int l, int r) {
    if (l > n || r > n || (op == 2 && l == r)) {
        return false;
    }
    if (find(l) == find(r)) {
        if (op == 1) {
            if (dist[l] != dist[r]) {
                return false;
            }
        } else {
            if ((dist[l] - dist[r] + 3) % 3 != 1) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    scanf("%d%d", &n, &k);
    prepare();
    for (int i = 1, op, l, r; i <= k; i++) {
        scanf("%d%d%d", &op, &l, &r);
        if (!check(op, l, r)) {
            ans++;
        } else {
            unionSet(op, l, r);
        }
    }
    printf("%d\n", ans);
    return 0;
}
