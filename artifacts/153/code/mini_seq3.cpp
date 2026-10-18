#include "sequence_impl2.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int main() {
    srand(20261018);
    memset(num,0,sizeof(num)); memset(fa,0,sizeof(fa));
    memset(ls,0,sizeof(ls)); memset(rs,0,sizeof(rs));
    memset(siz,0,sizeof(siz)); memset(sum,0,sizeof(sum));
    memset(all,0,sizeof(all)); memset(pre,0,sizeof(pre)); memset(suf,0,sizeof(suf));
    memset(update,0,sizeof(update)); memset(change,0,sizeof(change)); memset(rev,0,sizeof(rev));
    si = MAXN - 1; for (int i = 1; i <= si; i++) space[i] = i;
    head = 0;
    int n = rand() % 20 + 1;
    arr[1] = arr[n + 2] = -INF;
    vector<int> v;
    for (int i = 0; i < n; i++) { arr[i + 2] = rand() % 11 - 5; v.push_back(arr[i + 2]); }
    insert(0, n + 2);
    printf("init n=%d v:", n); for (int x : v) printf(" %d", x); printf(" siz[head]=%d\n", siz[head]);
    for (int k = 0; k < 2; k++) {
        int op = rand() % 6;
        printf("k%d raw op=%d v.size=%zu\n", k, op, v.size());
        int pos = 1, len = 1;
        if (!v.empty()) {
            if (v.size() == 1 && op >= 1 && op <= 4) op = 0;
            pos = rand() % (int)v.size() + 1;
            len = rand() % ((int)v.size() - pos + 1) + 1;
        } else op = 0;
        printf("  exec op=%d pos=%d len=%d\n", op, pos, len);
        if (op == 0) {
            int len2 = rand() % 10 + 1;
            vector<int> ins;
            for (int j = 1; j <= len2; j++) { arr[j] = rand() % 11 - 5; ins.push_back(arr[j]); }
            insert(pos + 1, len2);
            v.insert(v.begin() + pos - 1, ins.begin(), ins.end());
        } else if (op == 1) {
            remove(pos + 1, len);
            v.erase(v.begin() + pos - 1, v.begin() + pos - 1 + len);
        } else if (op == 2) {
            int c = rand() % 11 - 5;
            reset(pos + 1, len, c);
            for (int i = pos - 1; i < pos - 1 + len; i++) v[i] = c;
        } else if (op == 3) {
            reverse(pos + 1, len);
            std::reverse(v.begin() + pos - 1, v.begin() + pos - 1 + len);
        } else if (op == 4) {
            long long got = querySum(pos + 1, len);
            long long want = 0; for (int i = pos - 1; i < pos - 1 + len; i++) want += v[i];
            printf("  GETSUM got=%lld want=%lld\n", got, want);
        } else {
            long long got = queryMax();
            long long best = 0, cur = 0; for (int x : v) { cur += x; if (cur > best) best = cur; if (cur < 0) cur = 0; }
            printf("  MAXSUM got=%lld want=%lld\n", got, best);
        }
        printf("  after: v.size=%zu siz[head]=%d\n", v.size(), siz[head]);
    }
    return 0;
}
