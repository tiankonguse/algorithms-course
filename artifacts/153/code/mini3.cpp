#include "bookcase_impl2.h"
#include <cstdio>
#include <cstring>
void dump() {
    int stk[10], tp = 0, ii = head, out[10], oi = 0;
    while (tp != 0 || ii != 0) {
        if (ii != 0) { stk[++tp] = ii; ii = ls[ii]; }
        else { ii = stk[tp--]; out[oi++] = num[ii]; ii = rs[ii]; }
    }
    printf("   inord: "); for (int i = 0; i < oi; i++) printf("%d ", out[i]); printf(" head=%d fa1=%d fa2=%d fa3=%d\n", head, fa[1], fa[2], fa[3]);
}
int main() {
    memset(num,0,sizeof(num)); memset(fa,0,sizeof(fa));
    memset(ls,0,sizeof(ls)); memset(rs,0,sizeof(rs));
    memset(siz,0,sizeof(siz)); memset(pos,0,sizeof(pos));
    head=0; cnt=0;
    add(0); add(1); add(2);
    int ops[11] = {4,3,4,1,4,3,4,0,3,3,2};  // k0-k10
    for (int k = 0; k <= 10; k++) {
        int rank = ask(1) + 1;
        printf("k=%d op%d rank%d ", k, ops[k], rank); dump();
        if (ops[k] == 0) move(rank, 2);
        else if (ops[k] == 1) move(rank, 2);
        else if (ops[k] == 2) move(rank, rank);
        else if (ops[k] == 4) { int g = query(2); printf("   query=%d", g); dump(); }
        if (ops[k] == 0 || ops[k] == 1 || ops[k] == 2) { printf("   after move"); dump(); }
    }
    printf("all done\n");
    return 0;
}
