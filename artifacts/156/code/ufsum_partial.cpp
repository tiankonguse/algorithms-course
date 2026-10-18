// 推导部分和，带权并查集模版题1
// n 个数字，下标 1~n；m 个区间累加和，q 个区间累加和查询
// 查询能推出答案就打印，否则 UNKNOWN
// 1 <= n, m, q <= 1e5，累加和不超过 long 范围
// 测试链接 : https://www.luogu.com.cn/problem/P8779
#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX;
const int MAXN = 100002;

int n, m, q;
int father[MAXN];
long long dist[MAXN];

void prepare() {
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

void unionSet(int l, int r, long long v) {
    int lf = find(l), rf = find(r);
    if (lf != rf) {
        father[lf] = rf;
        dist[lf] = v + dist[r] - dist[l];
    }
}

long long query(int l, int r) {
    if (find(l) != find(r)) {
        return INF;
    }
    return dist[l] - dist[r];
}

int main() {
    scanf("%d%d%d", &n, &m, &q);
    n++;
    prepare();
    for (int i = 1, l, r; i <= m; i++) {
        long long v;
        scanf("%d%d%lld", &l, &r, &v);
        r++;
        unionSet(l, r, v);
    }
    for (int i = 1, l, r; i <= q; i++) {
        scanf("%d%d", &l, &r);
        r++;
        long long v = query(l, r);
        if (v == INF) {
            printf("UNKNOWN\n");
        } else {
            printf("%lld\n", v);
        }
    }
    return 0;
}
