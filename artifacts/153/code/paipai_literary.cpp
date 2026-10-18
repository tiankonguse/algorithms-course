#include "literary_impl.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int main() {
    srand(20261018);
    int ROUNDS = 300, STEPS = 300;
    int fails = 0;
    for (int round = 0; round < ROUNDS; round++) {
        head = 0; cnt = 0; si = 0; ai = 0;
        memset(num, 0, sizeof(num)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs));
        memset(siz, 0, sizeof(siz)); memset(rev, 0, sizeof(rev));
        int n = rand() % 30 + 1;
        add(0);
        for (int i = 1; i <= n; i++) add(i);
        add(0);
        vector<int> ref;
        for (int i = 1; i <= n; i++) ref.push_back(i);
        for (int k = 0; k < STEPS; k++) {
            int l = rand() % n + 1, r = rand() % n + 1;
            if (l > r) { int t = l; l = r; r = t; }
            reverse(l + 1, r + 1);
            std::reverse(ref.begin() + l - 1, ref.begin() + r);
        }
        ai = 0;
        inorder();
        if (ai - 2 != n) { printf("FAIL r=%d ai-2=%d n=%d\n", round, ai - 2, n); fails++; continue; }
        for (int i = 0; i < n; i++) {
            if (ans[i + 2] != ref[i]) {
                printf("MISMATCH r=%d i=%d got=%d want=%d\n", round, i, ans[i + 2], ref[i]);
                fails++; break;
            }
        }
    }
    printf("literary P3391: %dx%d %s\n", ROUNDS, STEPS, fails ? "FAIL" : "OK");
    return 0;
}
