#include "followup_impl2.h"
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
using namespace std;
int main() {
    srand(20261018);
    int ROUNDS = 300, STEPS = 300;
    int fails = 0;
    for (int round = 0; round < ROUNDS; round++) {
        head = 0; cnt = 0;
        memset(key, 0, sizeof(key)); memset(fa, 0, sizeof(fa));
        memset(ls, 0, sizeof(ls)); memset(rs, 0, sizeof(rs)); memset(siz, 0, sizeof(siz));
        int n = rand() % 10 + 1;
        multiset<int> ms;
        for (int i = 0; i < n; i++) {
            int v = rand() % 100 + 1;
            add(v); ms.insert(v);
        }
        int lastAns = 0, ans = 0;
        for (int k = 0; k < STEPS; k++) {
            int op = rand() % 6 + 1;
            if (ms.empty()) op = 1; // 空集合只允许 add
            int x = 1;
            // 直接生成「解码后合法」的 x（强制在线的编码只影响输入流，不影响树操作正确性）
            if (op == 1) x = rand() % 100 + 1;
            else if (op == 2) { auto it = ms.begin(); advance(it, rand() % (int)ms.size()); x = *it; } // 保证删除存在
            else if (op == 3) x = rand() % 100 + 1;
            else if (op == 4) x = rand() % (int)ms.size() + 1; // 排名合法
            else if (op == 5) x = *ms.begin() + 1 + rand() % 60; // 保证前驱存在
            else x = rand() % *ms.rbegin() + 1; // 保证后继存在
            int want = 0;
            if (op == 1) {
                add(x); ms.insert(x);
            } else if (op == 2) {
                remove(x);
                auto it = ms.find(x);
                if (it != ms.end()) ms.erase(it);
            } else if (op == 3) { // getRank: < x 的个数 + 1
                lastAns = getRank(x);
                want = (int)distance(ms.begin(), ms.lower_bound(x)) + 1;
                if (lastAns != want) { printf("MISMATCH r=%d k=%d rank x=%d got=%d want=%d\n", round, k, x, lastAns, want); fails++; goto next; }
                ans ^= lastAns;
            } else if (op == 4) { // index: 第 x 小（x 必须 ≤ 集合大小，强制在线重试保证合法）
                lastAns = index(x);
                auto it = ms.begin(); advance(it, x - 1);
                want = *it;
                if (lastAns != want) { printf("MISMATCH r=%d k=%d index x=%d got=%d want=%d\n", round, k, x, lastAns, want); fails++; goto next; }
                ans ^= lastAns;
            } else if (op == 5) { // pre: < x 最大（保证前驱存在：x > 最小元素）
                lastAns = pre(x);
                auto it = ms.lower_bound(x);
                want = (it == ms.begin()) ? INT_MIN : *prev(it);
                if (lastAns != want) { printf("MISMATCH r=%d k=%d pre x=%d got=%d want=%d\n", round, k, x, lastAns, want); fails++; goto next; }
                ans ^= lastAns;
            } else { // post: > x 最小（保证后继存在：x < 最大元素）
                lastAns = post(x);
                auto it = ms.upper_bound(x);
                want = (it == ms.end()) ? INT_MAX : *it;
                if (lastAns != want) { printf("MISMATCH r=%d k=%d post x=%d got=%d want=%d\n", round, k, x, lastAns, want); fails++; goto next; }
                ans ^= lastAns;
            }
        }
        // 最终 ans 已随每次查询累计（暴力每次查询后 ans ^= want，课程同样）
        next:;
    }
    printf("followup P6136: %dx%d %s\n", ROUNDS, STEPS, fails ? "FAIL" : "OK");
    return 0;
}
