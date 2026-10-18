#include "cashier_impl.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

int main() {
    srand(20261018);
    int FAILS = 0;
    for (int round = 0; round < 300; round++) {
        head = 0; cnt = 0; change = 0; enter = 0;
        memset(key, 0, sizeof(key)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs)); memset(siz, 0, sizeof(siz));
        limit = rand() % 40 + 1;
        vector<int> ref; // 绝对工资
        int ops = 300;
        for (int k = 0; k < ops; k++) {
            int t = rand() % 4; // 0 I 1 A 2 S 3 F
            int x = rand() % 60 - 20;
            if (t == 3) x = rand() % 40 + 1; // F x 为正整数（题面语义）
            if (t == 0) { // I x
                if (x >= limit) { enter++; add(x - change); ref.push_back(x); }
            } else if (t == 1) { // A x
                change += x;
                for (auto &v : ref) v += x;
            } else if (t == 2) { // S x
                change -= x;
                for (auto &v : ref) v -= x;
                departure();
                vector<int> keep;
                for (auto v : ref) if (v >= limit) keep.push_back(v);
                ref.swap(keep);
            } else { // F x：第 x 多
                int a = (x > siz[head]) ? -1 : index(siz[head] - x + 1) + change;
                int b;
                if (x > (int)ref.size()) b = -1;
                else {
                    sort(ref.begin(), ref.end(), greater<int>());
                    b = ref[x - 1];
                }
                if (a != b) {
                    printf("MISMATCH r=%d k=%d F x=%d got=%d want=%d (size=%d) change=%d\n", round, k, x, a, b, (int)ref.size(), change);
                    // dump 课程树中序 key
                    printf("  tree keys:");
                    for (int ii = 1; ii <= cnt; ii++) if (fa[ii] == 0) { /* root */ }
                    int stk[100], tp = 0, ii = head;
                    while (tp != 0 || ii != 0) { if (ii != 0) { stk[++tp] = ii; ii = ls[ii]; } else { ii = stk[tp--]; printf(" %d", key[ii]); ii = rs[ii]; } }
                    printf("\n  ref:");
                    for (auto v : ref) printf(" %d", v);
                    printf("\n");
                    FAILS++; goto next;
                }
            }
        }
        // 末尾离职人数 = enter - 当前人数
        if (enter - siz[head] != enter - (int)ref.size()) {
            printf("MISMATCH final r=%d got=%d want=%d\n", round, enter - siz[head], enter - (int)ref.size()); FAILS++;
        }
        next:;
        if (FAILS) break;
    }
    if (FAILS == 0) printf("cashier P1486: 300x300 OK\n");
    else printf("FAILS=%d\n", FAILS);
    return 0;
}
