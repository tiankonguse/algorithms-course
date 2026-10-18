#include "sequence_impl2.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
using namespace std;
long long kadane(const vector<int>& v) {  // 允许空段：全负输出 0（课程语义）
    long long best = 0, cur = 0;
    for (int x : v) { cur += x; if (cur > best) best = cur; if (cur < 0) cur = 0; }
    return best;
}
int main() {
    srand(20261018);
    int ROUNDS = 50, STEPS = 150;
    int fails = 0;
    for (int round = 0; round < ROUNDS; round++) {
        memset(num, 0, sizeof(num)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs));
        memset(siz, 0, sizeof(siz)); memset(sum, 0, sizeof(sum));
        memset(all, 0, sizeof(all)); memset(pre, 0, sizeof(pre)); memset(suf, 0, sizeof(suf));
        memset(update, 0, sizeof(update)); memset(change, 0, sizeof(change)); memset(rev, 0, sizeof(rev));
        si = MAXN - 1;
        for (int i = 1; i <= si; i++) space[i] = i;
        head = 0;
        int n = rand() % 20 + 1;
        arr[1] = arr[n + 2] = -INF;
        vector<int> v;
        for (int i = 0; i < n; i++) { arr[i + 2] = rand() % 11 - 5; v.push_back(arr[i + 2]); }
        insert(0, n + 2);
        for (int k = 0; k < STEPS; k++) {
            int op = rand() % 6;
                                    struct timeval __t0, __t1; gettimeofday(&__t0, 0);
            int pos = 1, len = 1;
            if (v.empty()) {
                op = 0; pos = 0; // 空序列只允许 INSERT（题面 pos=0：最前）
            } else if (op == 0) {
                pos = rand() % ((int)v.size() + 1); // INSERT：第 pos 个之后，pos∈[0,n]
                len = 1; // len 由 len2 决定
            } else {
                if (v.size() == 1 && op >= 1 && op <= 4) op = 0; // 单元素区间全操作等价，减少重复
                pos = rand() % (int)v.size() + 1;
                len = rand() % ((int)v.size() - pos + 1) + 1;
            }
            if (op == 0) { // INSERT
                int len2 = rand() % 10 + 1;
                vector<int> ins;
                for (int j = 1; j <= len2; j++) { arr[j] = rand() % 11 - 5; ins.push_back(arr[j]); }
                insert(pos + 1, len2);
                v.insert(v.begin() + pos, ins.begin(), ins.end()); // 课程插到「第 pos 个之后」（main pos++ 后 insert(pos+1)）
            } else if (op == 1) { // DELETE
                remove(pos + 1, len);
                v.erase(v.begin() + pos - 1, v.begin() + pos - 1 + len);
            } else if (op == 2) { // MAKE-SAME
                int c = rand() % 11 - 5;
                reset(pos + 1, len, c);
                for (int i = pos - 1; i < pos - 1 + len; i++) v[i] = c;
            } else if (op == 3) { // REVERSE
                reverse(pos + 1, len);
                std::reverse(v.begin() + pos - 1, v.begin() + pos - 1 + len);
            } else if (op == 4) { // GET-SUM
                long long got = querySum(pos + 1, len);
                long long want = 0; for (int i = pos - 1; i < pos - 1 + len; i++) want += v[i];
                if (got != want) { printf("MISMATCH r=%d k=%d GETSUM pos=%d len=%d got=%lld want=%lld\n", round, k, pos, len, got, want); fails++; goto next; }
            } else { // MAX-SUM
                                long long got = queryMax();
                long long want = kadane(v);
                if (got != want) { printf("MISMATCH r=%d k=%d MAXSUM got=%lld want=%lld n=%zu\n", round, k, got, want, v.size()); fails++; goto next; }
            }
            gettimeofday(&__t1, 0);
            long __ms = (__t1.tv_sec - __t0.tv_sec) * 1000 + (__t1.tv_usec - __t0.tv_usec) / 1000;
            if (__ms > 50) { fprintf(stderr, "SLOW k%d op%d %ldms n%zu\n", k, op, __ms, v.size()); fflush(stderr); }
            if (siz[head] != (int)v.size() + 2) { printf("SIZE r=%d k=%d got=%d want=%zu\n", round, k, siz[head], v.size() + 2); fails++; goto next; }
        }
        if (round % 5 == 4) { fprintf(stderr, "round %d done\n", round); fflush(stderr); }
        next:;
    }
    printf("sequence P2042: %dx%d %s\n", ROUNDS, STEPS, fails ? "FAIL" : "OK");
    return 0;
}
