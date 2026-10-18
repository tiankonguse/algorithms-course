// descartes 对拍：随机数组 → 课程建树 → 校验 BST(中序=1..n) + 小根堆(父值<=子值)
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1005;
int arr[MAXN], ls[MAXN], rs[MAXN], sta[MAXN], n;

void build() {
    int top = 0;
    for (int i = 1; i <= n; i++) {
        int pos = top;
        while (pos > 0 && arr[sta[pos]] > arr[i]) pos--;
        if (pos > 0) rs[sta[pos]] = i;
        if (pos < top) ls[i] = sta[pos + 1];
        sta[++pos] = i;
        top = pos;
    }
}

int inorder[MAXN], icnt;
void collect(int u) {
    if (!u) return;
    collect(ls[u]); inorder[++icnt] = u; collect(rs[u]);
}

bool check(int u) { // 堆性质
    if (!u) return true;
    if (ls[u] && arr[ls[u]] < arr[u]) return false;
    if (rs[u] && arr[rs[u]] < arr[u]) return false;
    return check(ls[u]) && check(rs[u]);
}

int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 300; round++) {
        n = 1 + rand() % 20;
        for (int i = 1; i <= n; i++) arr[i] = rand() % 10;
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs));
        build();
        icnt = 0; collect(sta[1]);
        bool ok = true;
        for (int i = 1; i <= n; i++) if (inorder[i] != i) ok = false; // BST 中序
        if (!check(sta[1])) ok = false;
        // 根：整段最小值里最左的（等值不弹 → 先出现者在上）
        int minv = arr[1], minpos = 1;
        for (int i = 2; i <= n; i++) if (arr[i] < minv) { minv = arr[i]; minpos = i; }
        if (sta[1] != minpos) ok = false;
        total++;
        if (!ok) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d n=%d arr:", round, n);
                for (int i = 1; i <= n; i++) printf("%d ", arr[i]);
                printf("root=%d expect=%d\n", sta[1], minpos);
            }
        }
    }
    printf("== descartes: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
