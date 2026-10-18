// treeorder P1377 对拍：输入排列 → 课程输出先序 → 暴力验证字典序最小 + 形态一致
#include <bits/stdc++.h>
using namespace std;

int arr[15], ls[15], rs[15], sta[15], n;
int order[15]; // 课程输出

void build() {
    for (int i = 1, top = 0, pos = 0; i <= n; i++) {
        pos = top;
        while (pos > 0 && arr[sta[pos]] > arr[i]) pos--;
        if (pos > 0) rs[sta[pos]] = i;
        if (pos < top) ls[i] = sta[pos + 1];
        sta[++pos] = i;
        top = pos;
    }
}
void pre() {
    int size = 1, i = 0, cur;
    while (size > 0) {
        cur = sta[size--];
        arr[++i] = cur;
        if (rs[cur]) sta[++size] = rs[cur];
        if (ls[cur]) sta[++size] = ls[cur];
    }
}

// 用插入序列 seq 建 BST，返回形态编码（先序节点序列）
string buildBST(const vector<int>& seq) {
    vector<int> L(n + 1, 0), R(n + 1, 0);
    int root = 0;
    for (int v : seq) {
        if (root == 0) { root = v; continue; }
        int cur = root;
        while (true) {
            if (v < cur) {
                if (L[cur]) cur = L[cur];
                else { L[cur] = v; break; }
            } else {
                if (R[cur]) cur = R[cur];
                else { R[cur] = v; break; }
            }
        }
    }
    // 先序编码
    string s;
    vector<int> st = {root};
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        s += char('0' + u);
        if (R[u]) st.push_back(R[u]);
        if (L[u]) st.push_back(L[u]);
    }
    return s;
}

int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 500; round++) {
        n = 1 + rand() % 8;
        // 生成输入排列（第 i 次插入的数值）
        vector<int> perm(n);
        for (int i = 0; i < n; i++) perm[i] = i + 1;
        random_shuffle(perm.begin(), perm.end());
        // 课程：arr[val] = 时间
        for (int i = 1; i <= n; i++) arr[i] = 0;
        for (int i = 0; i < n; i++) arr[perm[i]] = i + 1;
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs));
        build();
        pre();
        vector<int> got;
        for (int i = 1; i <= n; i++) got.push_back(arr[i]);
        // 原序列对应形态
        string origShape = buildBST(perm);
        // 课程输出序列的形态
        string gotShape = buildBST(got);
        // 字典序最小：枚举所有排列，找形态相同且字典序最小的
        vector<int> best = got;
        vector<int> p(n);
        for (int i = 0; i < n; i++) p[i] = i + 1;
        do {
            string sh = buildBST(p);
            if (sh == origShape && p < best) best = p;
        } while (next_permutation(p.begin(), p.end()));
        total++;
        bool ok = (gotShape == origShape) && (got == best);
        if (!ok) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d n=%d perm:", round, n);
                for (int v : perm) printf("%d ", v);
                printf("\n  got:"); for (int v : got) printf("%d ", v);
                printf("\n  best:"); for (int v : best) printf("%d ", v);
                printf(" shapeOk=%d\n", gotShape == origShape);
            }
        }
    }
    printf("== treeorder: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
