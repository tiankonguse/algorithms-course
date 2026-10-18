#include "bookcase_impl2.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
int main() {
    srand(20261018);
    int ROUNDS = 200, STEPS = 400;
    int fails = 0;
    for (int round = 0; round < ROUNDS; round++) {
        head = 0; cnt = 0;
        memset(num, 0, sizeof(num)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs));
        memset(siz, 0, sizeof(siz)); memset(pos, 0, sizeof(pos));
        int n = rand() % 30 + 1;
        add(0);
        for (int i = 1; i <= n; i++) add(i);
        add(n + 1);
        int tn = n + 2;
        vector<int> ref;
        for (int i = 1; i <= n; i++) ref.push_back(i);
        auto posOf = [&](int s) -> int { for (int i = 0; i < (int)ref.size(); i++) if (ref[i] == s) return i; return -1; };
        for (int k = 0; k < STEPS; k++) {
            int op = rand() % 5;
            int s = rand() % n + 1;
            int t = rand() % (2 * n + 1) - n;
            int rank = ask(s) + 1;
            fprintf(stderr, "r%d k%d op%d s%d t%d rank%d tn%d head%d sz%d\n", round, k, op, s, t, rank, tn, head, siz[head]);
            fflush(stderr);
            if (op == 2) { // Insert 的 t 保证 rank+t 在 [2, tn-1] 合法排名
                t = rand() % (tn - 2) + 2 - rank;
            }
            if (op == 0) { // Top
                move(rank, 2);
                int p = posOf(s); ref.erase(ref.begin() + p); ref.insert(ref.begin(), s);
            } else if (op == 1) { // Bottom
                move(rank, tn - 1);
                int p = posOf(s); ref.erase(ref.begin() + p); ref.push_back(s);
            } else if (op == 2) { // Insert
                move(rank, rank + t);
                int p = posOf(s); ref.erase(ref.begin() + p);
                int idx = p + t; if (idx < 0) idx = 0; if (idx > (int)ref.size()) idx = ref.size();
                ref.insert(ref.begin() + idx, s);
            } else if (op == 3) { // Ask
                int got = rank - 2;
                int want = posOf(s);
                if (got != want) { printf("MISMATCH r=%d k=%d Ask s=%d got=%d want=%d\n", round, k, s, got, want); fails++; goto next; }
            } else { // Query
                int got = query(s + 1);
                int want = ref[s - 1];
                if (got != want) { printf("MISMATCH r=%d k=%d Query s=%d got=%d want=%d\n", round, k, s, got, want); fails++; goto next; }
            }
        }
        // 全序列校验：抽树中序
        {
            int stk[500], tp = 0, ii = head, out[500], oi = 0;
            while (tp != 0 || ii != 0) {
                if (ii != 0) { stk[++tp] = ii; ii = ls[ii]; }
                else { ii = stk[tp--]; out[oi++] = num[ii]; ii = rs[ii]; }
            }
            // 跳过虚拟 0 和 n+1（num 值过滤）
            int fi = 0;
            for (int i = 0; i < oi; i++) {
                if (out[i] >= 1 && out[i] <= n) {
                    if (out[i] != ref[fi]) { printf("SEQ MISMATCH r=%d i=%d got=%d want=%d\n", round, fi, out[i], ref[fi]); fails++; break; }
                    fi++;
                }
            }
            if (fi != n && fails == 0) { printf("SEQ LEN r=%d fi=%d n=%d\n", round, fi, n); fails++; }
        }
        next:;
    }
    printf("bookcase P2596: %dx%d %s\n", ROUNDS, STEPS, fails ? "FAIL" : "OK");
    return 0;
}
