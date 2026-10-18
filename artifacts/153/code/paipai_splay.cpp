#include "splay_impl.h"
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

int main() {
    srand(20261018);
    int FAILS = 0;
    for (int round = 0; round < 400; round++) {
        // 重置课程端
        head = 0; cnt = 0;
        memset(key, 0, sizeof(key)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs)); memset(siz, 0, sizeof(siz));
        multiset<int> ref;
        int ops = 300;
        for (int k = 0; k < ops; k++) {
            int op = rand() % 6 + 1;
            int x = rand() % 41 - 20;
            if (op == 4) { // index：x 必须在集合大小内
                if (ref.empty()) op = 1;
                else x = rand() % ref.size() + 1;
            } else if (op == 2) {
                if (ref.empty()) op = 1;
                else if (ref.find(x) == ref.end()) x = *ref.begin(); // 删存在的
            }
            if (op == 1) {
                add(x); ref.insert(x);
            } else if (op == 2) {
                remove(x); ref.erase(ref.find(x));
            } else if (op == 3) {
                int a = getRank(x);
                int b = (int)ref.size() - (int)distance(ref.lower_bound(x), ref.end()) + 1;
                if (a != b) { printf("MISMATCH round=%d step=%d op3 x=%d got=%d want=%d\n", round, k, x, a, b); FAILS++; goto next; }
            } else if (op == 4) {
                int a = index(x);
                auto it = ref.begin(); advance(it, x - 1);
                if (a != *it) { printf("MISMATCH round=%d step=%d op4 x=%d got=%d want=%d\n", round, k, x, a, *it); FAILS++; goto next; }
            } else if (op == 5) {
                int a = pre(x);
                int want = INT_MIN;
                auto it = ref.lower_bound(x);
                if (it != ref.begin()) { --it; want = *it; }
                if (a != want) { printf("MISMATCH round=%d step=%d op5 x=%d got=%d want=%d\n", round, k, x, a, want); FAILS++; goto next; }
            } else {
                int a = post(x);
                int want = INT_MAX;
                auto it = ref.upper_bound(x);
                if (it != ref.end()) want = *it;
                if (a != want) { printf("MISMATCH round=%d step=%d op6 x=%d got=%d want=%d\n", round, k, x, a, want); FAILS++; goto next; }
            }
        }
        next:;
        if (FAILS) break;
    }
    if (FAILS == 0) printf("splay P3369 无词频: 400x300 OK\n");
    else printf("FAILS=%d\n", FAILS);
    return 0;
}
