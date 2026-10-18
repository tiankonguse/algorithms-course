#include "sequence_impl.h"
#include <cstdio>
#include <cstring>
int main() {
    memset(num,0,sizeof(num)); memset(fa,0,sizeof(fa));
    memset(ls,0,sizeof(ls)); memset(rs,0,sizeof(rs));
    memset(siz,0,sizeof(siz)); memset(sum,0,sizeof(sum));
    memset(all,0,sizeof(all)); memset(pre,0,sizeof(pre)); memset(suf,0,sizeof(suf));
    memset(update,0,sizeof(update)); memset(change,0,sizeof(change)); memset(rev,0,sizeof(rev));
    si = MAXN - 1; for (int i = 1; i <= si; i++) space[i] = i;
    head = 0;
    int n = 1;
    arr[1] = arr[n+2] = -INF;
    arr[2] = -1;
    insert(0, n+2);
    printf("head=%d all=%d sum=%d siz=%d\n", head, all[head], sum[head], siz[head]);
    printf("queryMax=%d\n", queryMax());
    return 0;
}
